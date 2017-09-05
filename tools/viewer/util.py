from PyQt5.QtCore import (
    Qt,
    QPointF
)
from PyQt5.QtGui import (
    QPolygonF
)


class Converter(object):

    def __init__(self, data=None):
        if data is not None:
            self.set_data(data)

    def set_data(self, data):
        data_list = data.split(':')
        self.number = int(data_list.pop(0))
        self.pieces = [self.to_polygon(d) for d in data_list[:-1]]
        self.frame = self.to_polygon(data_list[-1])

    def to_points(self, data):
        if not isinstance(data, str):
            return None
        points = data.split()
        length = int(points.pop(0)) * 2

        result = []
        for i in range(0, length, 2):
            x = int(points[i])
            y = int(points[i + 1])
            result.append(QPointF(x, y))

        return result

    def to_polygon(self, data):
        points = self.to_points(data)
        if points is None:
            return None

        points.append(points[0])
        return QPolygonF(points)
