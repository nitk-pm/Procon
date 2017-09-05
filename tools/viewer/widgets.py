from PyQt5.QtCore import (
    Qt,
    QSize,
    pyqtSlot
)
from PyQt5.QtWidgets import (
    QMainWindow,
    QWidget,
    QFrame,
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


class PolygonWidget(QFrame):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setStyleSheet('background-color: rgb(66, 66, 66);margin: 5px;')
        self.setFrameShape(QFrame.Panel)
        self.setFrameShadow(QFrame.Raised)
        self.setLineWidth(2)
        self.setMidLineWidth(2)

        self.polygon = None
        self.scale = 1
        self.margin = 10

    def set_polygon(self, polygon):
        self.polygon = polygon
        self.update_size()

    def set_scale(self, scale):
        self.scale = scale
        self.update_size()

    def update_size(self):
        w = self.polygon.boundingRect().width()
        h = self.polygon.boundingRect().height()
        self.resize(
            w * self.scale + self.margin * 2,
            h * self.scale + self.margin * 2
        )

    def paintEvent(self, event):
        painter = QPainter(self)

        opt = QStyleOption()
        opt.initFrom(self)
        self.style().drawPrimitive(QStyle.PE_Frame, opt, painter, self)

        if self.polygon is not None:
            source = self.polygon.boundingRect()
            delta_x = self.rect().width() - source.width() * self.scale
            delta_y = self.rect().height() - source.height() * self.scale

            painter.translate(delta_x / 2, delta_y / 2)
            painter.scale(self.scale, self.scale)
            painter.setPen(QPen(Qt.white, 0.5))
            painter.drawPolygon(self.polygon)


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('form.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)
        self.number = 0
        self.dir_path = ''
        self.filename = ''

        self.ui.open.clicked.connect(self.open_file)

    @pyqtSlot()
    def open_file(self):
        import os
        from PyQt5.QtWidgets import QFileDialog
        filename = QFileDialog.getOpenFileName(
            self,
            'Read File',
            self.dir_path,
            'official format (*.txt)'
        )

        if filename[0] == '':
            return

        with open(filename[0]) as file:
            data = file.read()
            converter = Converter(data)

            self.ui.piece_view.clear()
            for piece in converter.pieces:
                self.add_piece(piece)

            self.ui.frame_view.set_polygon(converter.frame)
            self.ui.frame_view.set_scale(6)

            self.dir_path, self.filename = os.path.split(filename[0])
            self.ui.filename.setText(self.filename)
            self.ui.pieces.display(converter.number)

    def add_piece(self, piece):
        widget = PolygonWidget()
        widget.set_polygon(piece)
        widget.set_scale(6)
        s = widget.size()
        s.setWidth(s.width() + widget.margin * 2)
        s.setHeight(s.height() + widget.margin * 2)
        item = QListWidgetItem()
        item.setSizeHint(s)
        self.ui.piece_view.addItem(item)
        self.ui.piece_view.setItemWidget(item, widget)
