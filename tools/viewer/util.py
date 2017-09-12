from PyQt5.QtCore import (
    Qt,
    QPointF
)
from PyQt5.QtGui import (
    QPolygonF,
    QTransform
)


class ProblemDecoder(object):

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
