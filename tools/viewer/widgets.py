from PyQt5.QtCore import (
    Qt,
    QSize,
    pyqtSlot,
)
from PyQt5.QtWidgets import (
    QMainWindow,
    QWidget,
    QFrame,
    QListWidgetItem,
    QStyle,
    QStyleOption,
    QPushButton,
    QGridLayout,
    QHBoxLayout
)
from PyQt5.QtGui import (
    QPainter,
    QPen,
    QPixmap,
    QIcon
)
from PyQt5 import uic
from models import ProblemData


class PolygonWidget(QFrame):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setStyleSheet('background-color: rgb(66, 66, 66);margin: 5px;')
        self.setFrameShape(QFrame.Panel)
        self.setFrameShadow(QFrame.Raised)
        self.setLineWidth(2)
        self.setMidLineWidth(2)

        self.polygons = []
        self.scale = 1
        self.margin = 10

    def set_polygons(self, polygons):
        self.polygons = polygons
        self.update_size()

    def set_scale(self, scale):
        self.scale = scale
        self.update_size()

    def update_size(self):
        w = self.polygons[0].boundingRect().width()
        h = self.polygons[0].boundingRect().height()
        for polygon in self.polygons[1:]:
            if w < polygon.boundingRect().width():
                w = polygon.boundingRect().width()
            if h < polygon.boundingRect().height():
                h = polygon.boundingRect().height()
        self.resize(
            w * self.scale + self.margin * 2,
            h * self.scale + self.margin * 2
        )


class FrameWidget(PolygonWidget):

    def paintEvent(self, event):
        painter = QPainter(self)

        opt = QStyleOption()
        opt.initFrom(self)
        self.style().drawPrimitive(QStyle.PE_Frame, opt, painter, self)

        for polygon in self.polygons:
            source = polygon.boundingRect()
            delta_x = self.rect().width() - source.width() * self.scale
            delta_y = self.rect().height() - source.height() * self.scale

            painter.translate(delta_x / 2, delta_y / 2)
            painter.scale(self.scale, self.scale)
            painter.setPen(QPen(Qt.white, 0.5))
            painter.drawPolygon(polygon)


class PieceWidget(PolygonWidget):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.create_ui()
        self.index = 0

    def create_ui(self):
        import viewer_rc
        self.setLayout(QHBoxLayout(self))
        self.layout().setContentsMargins(0, 0, 0, 0)
        self.right = QPushButton(self)
        self.right.setMaximumSize(QSize(40, 1677225))
        self.right.setMinimumSize(QSize(40, 0))
        self.right.setIcon(QIcon(QPixmap(':/icons/icons/right.png')))
        self.right.setIconSize(QSize(30, 30))
        self.left = QPushButton(self)
        self.left.setMaximumSize(QSize(40, 1677225))
        self.left.setMinimumSize(QSize(40, 0))
        self.left.setIcon(QIcon(QPixmap(':/icons/icons/left.png')))
        self.left.setIconSize(QSize(30, 30))
        self.layout().addWidget(self.left)
        self.layout().addWidget(self.right)
        self.right.clicked.connect(self.increment)
        self.left.clicked.connect(self.decrement)

    @pyqtSlot()
    def increment(self):
        self.index += 1
        if self.index >= 8:
            self.index = 0
        self.update()

    @pyqtSlot()
    def decrement(self):
        self.index -= 1
        if self.index <= -1:
            self.index = 7
        self.update()

    def resizeEvent(self, event):
        super().resizeEvent(event)
        button_w = self.right.width() + self.left.width() + self.margin
        self.layout().setSpacing(self.width() - button_w)

    def paintEvent(self, event):
        super().paintEvent(event)
        painter = QPainter(self)

        opt = QStyleOption()
        opt.initFrom(self)
        self.style().drawPrimitive(QStyle.PE_Frame, opt, painter, self)

        source = self.polygons[self.index].boundingRect()
        delta_x = self.rect().width() - source.width() * self.scale
        delta_y = self.rect().height() - source.height() * self.scale

        painter.translate(delta_x / 2, delta_y / 2)
        painter.scale(self.scale, self.scale)
        painter.setPen(QPen(Qt.white, 0.5))
        painter.drawPolygon(self.polygons[self.index])


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('form2.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)

        self.ui.open_file.clicked.connect(self.open_file)
        self.ui.problem_list.currentTextChanged.connect(self.load_data)

    @pyqtSlot()
    def open_file(self):
        import os
        from PyQt5.QtWidgets import QFileDialog
        filename = QFileDialog.getOpenFileName(
            self,
            'Read File',
            '',
            'official format (*.txt)'
        )[0]

        if filename == '':
            return

        with open(filename) as file:
            data = file.read()
            problem_data = ProblemData(filename, data)
            self.ui.problem_list.addItem(
                problem_data.display_name,
                problem_data
            )
            self.ui.problem_list.setCurrentText(problem_data.display_name)

    @pyqtSlot(str)
    def load_data(self, text):
        problem_data = self.ui.problem_list.currentData()
        self.ui.piece_view.clear()
        for piece in problem_data.pieces:
            self.add_piece(piece)

        self.ui.frame_view.set_polygons(problem_data.frame)
        self.ui.frame_view.set_scale(6)

        self.ui.num_piece.setText('{}'.format(problem_data.num_piece))

    def add_piece(self, piece):
        widget = PieceWidget()
        widget.set_polygons(piece)
        widget.set_scale(6)
        s = widget.size()
        s.setWidth(s.width() + widget.margin * 2)
        s.setHeight(s.height() + widget.margin * 2)
        item = QListWidgetItem()
        item.setSizeHint(s)
        self.ui.piece_view.addItem(item)
        self.ui.piece_view.setItemWidget(item, widget)
