from PyQt5.QtCore import (
    Qt,
    QPointF,
    QRectF,
    QSizeF
)
from PyQt5.QtWidgets import (
    QGraphicsPixmapItem
)


class Frame(QGraphicsPixmapItem):

    def __init__(self, width=101, height=65, base=40, parent=None):
        super().__init__(parent)
        self.base = base
        self.radius = base / 10
        self.center = QPointF(base / 2, base / 2)
        self.grid_size = QSizeF(width, height)
        self.image_size = QSizeF(width * base, height * base)

        from PyQt5.QtGui import QImage, QPixmap, QPainter
        from itertools import product
        pixmap = QPixmap(self.image_size.width(), self.image_size.height())
        pixmap.fill()

        painter = QPainter()
        painter.begin(pixmap)
        painter.setBrush(Qt.black)

        for y, x in product(range(height), range(width)):
            offset = self.center + QPointF(base * x, base * y)
            painter.drawEllipse(offset, self.radius, self.radius)

        painter.end()

        self.setPixmap(pixmap)
        self.area = QRectF(
            0,
            0,
            self.image_size.width(),
            self.image_size.height()
        )

    def map_to_grid(self, pos):
        p = self.mapFromScene(pos) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def map_from_grid(self, pos):
        return self.mapToScene(pos * self.base) + self.center

    def snap_to_grid(self, pos):
        p = self.map_to_grid(pos)
        return self.map_from_grid(p)

    def contains(self, pos):
        return self.area.contains(pos)