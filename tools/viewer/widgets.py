import os
from PyQt5.QtCore import (
    Qt,
    QSize,
    pyqtSlot,
    QPointF
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
    QHBoxLayout,
    QFileDialog
)
from PyQt5.QtGui import (
    QPainter,
    QPen,
    QPixmap,
    QIcon
)
from PyQt5 import uic
from models import ProblemData
from watcher import Watcher


class PolygonWidget(QFrame):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setStyleSheet('background-color: rgb(66, 66, 66);margin: 5px;')
        self.setFrameShape(QFrame.Panel)
        self.setFrameShadow(QFrame.Raised)
        self.setLineWidth(2)
        self.setMidLineWidth(2)

        self.polygons = []
        self.united_rect = None
        self.scale = 1
        self.margin = 10

    def set_polygons(self, polygons):
        self.polygons = polygons
        self.update_size()

    def set_scale(self, scale):
        self.scale = scale
        self.update_size()

    def update_size(self):
        self.united_rect = self.polygons[0].boundingRect()
        for polygon in self.polygons:
            self.united_rect = self.united_rect.united(polygon.boundingRect())
        self.update()

    def draw_polygon(self, painter):
        pass

    def paintEvent(self, event):
        super().paintEvent(event)
        painter = QPainter(self)
        opt = QStyleOption()
        opt.initFrom(self)
        self.style().drawPrimitive(QStyle.PE_Frame, opt, painter, self)
        if len(self.polygons) != 0:
            self.draw_polygon(painter)


class FrameWidget(PolygonWidget):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.pieces = []
        self.offset = []

    def draw_polygon(self, painter):
        delta_x = self.rect().width() - self.united_rect.width() * self.scale
        delta_x /= 2
        delta_y = self.rect().height() - self.united_rect.height() * self.scale
        delta_y /= 2
        for polygon in self.polygons:
            source = polygon.boundingRect()

            painter.save()
            painter.translate(delta_x, delta_y)
            painter.scale(self.scale, self.scale)
            painter.setPen(QPen(Qt.white, 0.5))
            painter.drawPolygon(polygon)
            painter.restore()


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

    def update_size(self):
        super().update_size()
        self.resize(
            self.united_rect.width() * self.scale + self.margin * 2,
            self.united_rect.height() * self.scale + self.margin * 2
        )

    def draw_polygon(self, painter):
        source = self.polygons[self.index].boundingRect()
        delta_x = self.rect().width() - source.width() * self.scale
        delta_y = self.rect().height() - source.height() * self.scale

        painter.translate(delta_x / 2, delta_y / 2)
        painter.scale(self.scale, self.scale)
        painter.setPen(QPen(Qt.white, 0.5))
        painter.drawPolygon(self.polygons[self.index])

    def resizeEvent(self, event):
        super().resizeEvent(event)
        button_w = self.right.width() + self.left.width() + self.margin
        self.layout().setSpacing(self.width() - button_w)


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('form2.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)

        self.watcher = None

        self.ui.dir_path.setText('./')

        self.ui.play.clicked.connect(self.play)
        self.ui.stop.clicked.connect(self.stop)
        self.ui.reference.clicked.connect(self.reference)
        self.ui.open_file.clicked.connect(self.open_file)
        self.ui.export.clicked.connect(self.export)
        self.ui.problem_list.currentTextChanged.connect(self.display)

    def closeEvent(self, event):
        if self.watcher is not None:
            self.watcher.stop()
            del self.watcher

    @pyqtSlot()
    def play(self):
        self.ui.reference.setEnabled(False)
        self.watcher = Watcher(self.load)
        self.watcher.start(self.ui.dir_path.text())

    @pyqtSlot()
    def stop(self):
        self.ui.reference.setEnabled(True)
        self.watcher.stop()
        del self.watcher
        self.watcher = None

    @pyqtSlot()
    def reference(self):
        path = QFileDialog.getExistingDirectory(
            self,
            'Reference',
            self.ui.dir_path.text()
        )
        if path != '':
            self.ui.dir_path.setText(path)

    @pyqtSlot()
    def open_file(self):
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
            self.load(filename, data)

    @pyqtSlot(str)
    def display(self, text):
        problem_data = self.ui.problem_list.currentData()
        self.ui.piece_view.clear()
        for piece in problem_data.pieces:
            self.add_piece(piece)

        self.ui.frame_view.set_polygons(problem_data.frame)
        self.ui.frame_view.set_scale(6)
        self.ui.num_piece.setText('{}'.format(problem_data.num_piece))

    @pyqtSlot()
    def export(self):
        import json
        problem_data = self.ui.problem_list.currentData()
        piece = problem_data.to_dict_piece()
        frame = problem_data.to_dict_frame()
        with open('piece.json', 'w') as file:
            json.dump(piece, file, indent=2)
        with open('frame.json', 'w') as file:
            json.dump(frame, file, indent=2)

    def load(self, filename, data):
        problem_data = ProblemData(filename, data)
        self.ui.problem_list.addItem(
            problem_data.display_name,
            problem_data
        )
        self.ui.problem_list.setCurrentText(problem_data.display_name)

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
