from PyQt5.QtCore import (
    Qt,
    QPointF
)
from PyQt5.QtGui import (
    QPolygonF,
    QTransform
)


class Encoder(object):

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

    # 多角形のデータにデコード
    def to_polygon(self, data: str) -> QPolygonF:
        points = self.to_points(data)
        points.append(points[0])
        return QPolygonF(points)

    # 頂点リストにデコード
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
        c = polygon.boundingRect().center()
        trans = QTransform()
        trans.translate(c.x(), c.y())
        trans.rotate(angle)
        trans.translate(-c.x(), -c.y())
        return trans.map(polygon)

    # 左右反転
    def invert_polygon(self, polygon: QPolygonF) -> QPolygonF:
        width = polygon.boundingRect().width()
        trans = QTransform(-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0)
        trans.translate(-width, 0)
        return trans.map(polygon)

    def to_dict(self, data: str, is_frame: bool=False):
        result = None
        if is_frame:
            polygon = self.to_polygon(data)
            result = [{'x': int(p.x()), 'y': int(p.y())} for p in polygon]
        else:
            polygon_list = self.expand_patterns(data)
            result = {'shapes': []}
            for poly in polygon_list:
                d = [{'x': int(p.x()), 'y': int(p.y())} for p in poly[:-1]]
                result['shapes'].append(d)
        return result


if __name__ == '__main__':
    import json
    encoder = Encoder()
    piece = {'pieces': []}
    frame = {'shapes': []}
    while 1:
        try:
            data = input()
            data_list = data.split(':')
            print(data_list[0])
            num = int(data_list.pop(0))
            for d in data_list[:num]:
                piece['pieces'].append(encoder.to_dict(d))
            for d in data_list[num:]:
                frame['shapes'].append(encoder.to_dict(d, True))
        except EOFError:
            break
    with open('piece.json', 'w') as file:
        json.dump(piece, file, indent=2)
    with open('frame.json', 'w') as file:
        json.dump(frame, file, indent=2)
