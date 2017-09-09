from PyQt5.QtCore import (
    Qt,
    QPointF
)
from PyQt5.QtGui import (
    QPolygonF,
    QTransform
)


# 空白区切りの整数列から多角形オブジェクトを生成するコンバータ
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

    def expand_patterns(self, data):
        from PyQt5.QtGui import QTransform
        polygon = self.to_polygon(data)
        pattern_list = []
        for angle in [0, 90, 180, 270]:
            pattern_list.append(self.rotate_polygon(polygon, angle))
            pattern_list.append(self.inverse_polygon(pattern_list[-1]))
        for i, pattern in enumerate(pattern_list):
            print('pattern {}'.format(i + 1))
            for p in pattern:
                print('({}, {})'.format(p.x(), p.y()))

    def rotate_polygon(self, polygon, angle):
        c = polygon.boundingRect().center()
        trans = QTransform()
        trans.translate(c.x(), c.y())
        trans.rotate(angle)
        trans.translate(-c.x(), -c.y())
        return trans.map(polygon)

    def inverse_polygon(self, polygon):
        width = polygon.boundingRect().width()
        trans = QTransform()
        trans.setMatrix(
            -1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0
        )
        trans.translate(-width, 0)
        return trans.map(polygon)
