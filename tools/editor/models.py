from PyQt5.QtCore import (
    QObject
)
from PyQt5.QtWidgets import (
    QGraphicsItem,
    QGraphicsPixmapItem,
    QUndoStack
)


class Layer(QGraphicsItem):

    def __init__(self, name='layer', z_value=0, opacity=1.0, parent=None):
        super().__init__(parent)
        self.name = name
        self.setZValue(z_value)
        self.setOpacity(opacity)

    def add_item(self, item):
        if item not in self.childItems():
            item.setParentItem(self)
            self.scene().update()

    def remove_item(self, item):
        if item in self.childItems():
            item.setParentItem(None)
            self.scene().remove_item(item)


class Board(QGraphicsPixmapItem):

    def __init__(self, width=101, height=65, base=1, parent=None):
        super().__init__(parent)
        base *= 20
        self.base = base * 2
        self.width = (width + 1) * self.base
        self.height = (height + 1) * self.base

        from PyQt5.QtGui import QImage, QPixmap, QPainter
        from itertools import product
        pixmap = QPixmap(self.width, self.height)
        pixmap.fill()

        painter = QPainter()
        painter.begin(pixmap)
        painter.setBrush(Qt.black)

        b = base / 4
        for y, x in product(range(1, height + 1), range(1, width + 1)):
            painter.drawEllipse(x * self.base, y * self.base, b, b)

        painter.end()

        self.setPixmap(pixmap)
        self.area = QRectF(1, 1, self.width - 1, self.height - 1)

    def map_from_grid(self, pos):
        b = QPointF(self.base / 2, self.base / 2)
        p = self.mapFromScene(pos - b) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def map_to_grid(self, pos):
        p = self.map_from_grid(pos)
        p = self.mapToScene(p * self.base)
        return p + QPointF(self.base, self.base) * 1.075

    def contains(self, pos):
        return self.area.contains(pos)


class Document(QObject):

    def __init__(self, parent):
        super().__init__(parent)
        self.history = QUndoStack()
        self.vertex_layer = Layer(name='vertex', z_value=1, opacity=1.0)
        self.edge_layer = Layer(name='edge', z_value=1, opacity=1.0)
        self.vertexes = []
        self.edges = []

    def add_edge(self, src, dest):
        pass
