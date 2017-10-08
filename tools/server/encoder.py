from PyQt5.QtCore import (
    Qt,
    QPointF
)
from PyQt5.QtGui import (
    QPolygonF,
    QTransform
)
import warnings
import sys


class Encoder(object):

    def __init__(self, force: bool = True):
        self.force_encode = force

    # 8パターンに展開
    def expand_patterns(self, data: str) -> list:
        polygon = self.to_polygon(data)
        pattern_list = []
        invert_list = []
        for angle in [0, 90, 180, 270]:
            pattern_list.append(self.rotate_polygon(polygon, angle))
            invert_list.append(self.invert_polygon(pattern_list[-1]))
        pattern_list.extend(invert_list)
        return pattern_list

    # 多角形のデータにエンコード
    def to_polygon(self, data: str) -> QPolygonF:
        points = self.to_points(data)
        points.append(points[0])
        polygon = QPolygonF(points)
        if self.check_loopwise(polygon) < 0:
            if self.force_encode:
                warnings.warn(
                    'The direction of rotation is opposite: {}'.format(data)
                )
                polygon = QPolygonF(reversed(polygon))
            else:
                sys.stderr.write(
                    'The direction of rotation is opposite: {}'.format(data)
                )
                sys.exit()

        return polygon

    # 頂点リストにエンコード
    def to_points(self, data: str) -> list:
        data_list = data.split(' ')
        length = int(data_list.pop(0)) * 2
        points = []
        for i in range(0, length, 2):
            x, y = int(data_list[i]), int(data_list[i + 1])
            points.append(QPointF(x, y))
        return points

    # 指定した角度に回転
    def rotate_polygon(self, polygon: QPolygonF, angle: float) -> QPolygonF:
        trans = QTransform()
        trans.rotate(angle)
        poly = trans.map(polygon)
        return self.adjust(poly)

    # 左右反転
    def invert_polygon(self, polygon: QPolygonF) -> QPolygonF:
        width = polygon.boundingRect().width()
        points = []
        for p in reversed(polygon):
            points.append(QPointF(-p.x(), p.y()))
        poly = QPolygonF(points)
        return self.adjust(poly)

    # 座標の調整
    def adjust(self, polygon: QPolygonF):
        points = []
        rect = polygon.boundingRect()
        for p in polygon:
            points.append(p - rect.topLeft())
        return QPolygonF(points)

    # 時計回りか反時計回りか判定
    def check_loopwise(self, polygon: QPolygonF) -> int:
        p = polygon
        area = 0.0
        for i in range(len(p) - 1):
            cross = p[i].x() * p[i + 1].y() - p[i].y() * p[i + 1].x()
            area += cross
        area /= 2
        return -1 if area < 0 else 1

    # dict型で出力
    def to_dict(self, data: str, is_frame: bool=False):
        result = None
        if is_frame:
            polygon = self.to_polygon(data)
            result = [{'x': int(p.x()), 'y': int(p.y())} for p in polygon[:-1]]
        else:
            polygon_list = self.expand_patterns(data)
            result = {'shapes': []}
            for poly in polygon_list:
                d = [{'x': int(p.x()), 'y': int(p.y())} for p in poly[:-1]]
                result['shapes'].append(d)
        return result


if __name__ == '__main__':
    import json
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', action='store_true')
    argv = sys.argv
    argv.pop(0)
    parse = parser.parse_args(argv)
    encoder = Encoder(parse.f)

    piece = {'pieces': []}
    frame = {'shapes': []}
    while 1:
        try:
            data = input()
            data_list = data.split(':')
            num = int(data_list.pop(0))
            for d in data_list[:num]:
                piece['pieces'].append(encoder.to_dict(d))
            for d in data_list[num:]:
                frame['shapes'].append(encoder.to_dict(d, True))
        except EOFError:
            break
    with open('data/piece.json', 'w') as file:
        json.dump(piece, file, indent=2)
    with open('data/frame.json', 'w') as file:
        json.dump(frame, file, indent=2)
