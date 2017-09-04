from PyQt5.QtCore import (
    Qt,
    QSize
)
from PyQt5.QtWidgets import (
    QMainWindow,
    QWidget,
    QListWidgetItem,
    QStyle,
    QStyleOption
)
from PyQt5.QtGui import (
    QPainter,
    QPen,
)
from PyQt5 import uic
from util import Converter


class PolygonWidget(QWidget):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.polygon = None
        self.scale = 1

    def set_polygon(self, polygon):
        self.polygon = polygon
        width = self.polygon.boundingRect().width()
        height = self.polygon.boundingRect().height()
        self.resize(width * self.scale + 10, height * self.scale + 10)
        self.update()

    def set_scale(self, scale):
        self.scale = scale
        width = self.polygon.boundingRect().width()
        height = self.polygon.boundingRect().height()
        self.resize(width * self.scale + 10, height * self.scale + 10)
        self.update()

    def paintEvent(self, event):
        option = QStyleOption()
        option.initFrom(self)
        painter = QPainter(self)
        self.style().drawPrimitive(QStyle.PE_Widget, option, painter, self)
        painter.setPen(QPen(Qt.white, 0.5, cap=Qt.RoundCap))

        source = self.polygon.boundingRect()
        delta_x = self.rect().width() - source.width() * self.scale
        delta_y = self.rect().height() - source.height() * self.scale

        painter.translate(delta_x / 2, delta_y / 2)
        painter.scale(self.scale, self.scale)
        painter.drawPolygon(self.polygon)


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('form.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)
        self.title = self.windowTitle()
        self.converter = Converter()

        self.ui.frame_view.set_polygon(
            self.converter.to_polygon('4 0 64 100 64 100 0 0 0')
        )
        self.ui.frame_view.set_scale(6)

        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 100 0 100 5 5 100 0 100')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')
        self.add_piece('5 0 0 10 0 10 5 5 10 0 10')

    def add_piece(self, data):
        item = QListWidgetItem()
        widget = PolygonWidget()
        widget.set_polygon(
            self.converter.to_polygon(data)
        )
        widget.set_scale(6)
        item.setSizeHint(widget.size())
        self.ui.piece_view.addItem(item)
        self.ui.piece_view.setItemWidget(item, widget)
