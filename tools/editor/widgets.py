from PyQt5.QtCore import (
    Qt,
    QObject,
    QTimer,
    QTimeLine,
    pyqtSlot,
    pyqtSignal,
    QPointF,
    QRectF
)
from PyQt5.QtWidgets import (
    QGraphicsView,
    QGraphicsScene,
    QAction,
    QActionGroup,
    QGraphicsPixmapItem,
)
from PyQt5.QtGui import (
    QColor
)


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

    def map_to_grid(self, pos):
        b = QPointF(self.base / 2, self.base / 2)
        p = self.mapFromScene(pos - b) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def map_from_grid(self, pos):
        p = self.mapToScene(pos * self.base)
        return p + QPointF(self.base, self.base) * 1.075

    def snap_to_grid(self, pos):
        p = self.map_to_grid(pos)
        return self.map_from_grid(p)

    def contains(self, pos):
        return self.area.contains(pos)


class BoardScene(QGraphicsScene):

    show_message = pyqtSignal(str)

    def __init__(self, document, parent=None):
        super().__init__(parent)
        self.document = document
        self.board = Board()
        self.actions = QActionGroup(self)
        self.action_edge = None
        self.action_select = None
        self.action_delete = None

        self.addItem(self.board)
        self.addItem(self.document.node_layer)
        self.addItem(self.document.edge_layer)

        self.actions.setExclusive(True)
        self.selectionChanged.connect(self.select_controll)

    def setup_actions(self, actions):
        if 'mode' in actions:
            for action in actions['mode']:
                self.actions.addAction(action)
        if 'delete' in actions:
            self.action_delete = actions['delete']
            self.action_delete.triggered.connect(self.delete_objects)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.clearSelection()
        if event.key() == Qt.Key_A:
            self.output = self.document.output_data(remove=True)
        if event.key() == Qt.Key_B:
            print('restore')
            self.document.restore(self.output)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.grid_pos = self.board.map_to_grid(event.scenePos())
            pos = self.board.map_from_grid(self.grid_pos)
            if self.actions.checkedAction().text() == 'edge':
                self.document.create_node(pos)
            elif self.actions.checkedAction().text() == 'select':
                pass

        super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if event.buttons() & Qt.LeftButton and len(self.selectedItems()) != 0:
            p = self.board.map_to_grid(event.scenePos())
            d = p - self.grid_pos
            self.show_message.emit(
                'pos: ({:.0f}, {:.0f})  delta: ({:.0f}, {:.0f})'
                .format(p.x(), p.y(), d.x(), d.y())
            )
        else:
            p = self.board.map_to_grid(event.scenePos())
            self.show_message.emit(
                'pos: ({:.0f}, {:.0f})'
                .format(p.x(), p.y())
            )
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.LeftButton:
            if self.actions.checkedAction().text() == 'edge':
                self.document.merge_nodes()
                self.clearSelection()
            elif self.actions.checkedAction().text() == 'select':
                self.document.merge_nodes(self.selectedItems())
        self.show_message.emit('')
        super().mouseReleaseEvent(event)

    @pyqtSlot()
    def select_controll(self):
        self.action_delete.setEnabled(len(self.selectedItems()) != 0)

    @pyqtSlot()
    def delete_objects(self):
        self.document.remove_nodes(self.selectedItems())


class View(QGraphicsView):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setBackgroundBrush(QColor(66, 66, 66))
        self.setMouseTracking(True)
        self.scheduled_scalings = 0
        self.translate_scalings = 1.0
        self.can_move_scene = False
        self.old_pos = None

    def zoom(self, delta):
        degrees = int(delta / 8)
        steps = int(degrees / 15)
        self.scheduled_scalings += steps
        if self.scheduled_scalings * steps < 0:
            self.scheduled_scalings = steps
        anim = QTimeLine(350, self)
        anim.setUpdateInterval(20)
        anim.valueChanged.connect(self.scalingTime)
        anim.finished.connect(self.animFinished)
        anim.start()

    def wheelEvent(self, event):
        if event.modifiers() & Qt.ControlModifier:
            self.zoom(event.angleDelta().y())
        elif event.modifiers() & Qt.ShiftModifier:
            v = self.horizontalScrollBar().value() - event.angleDelta().y()
            self.horizontalScrollBar().setValue(v)
        else:
            super().wheelEvent(event)

    def mousePressEvent(self, event):
        if event.button() == Qt.RightButton:
            self.old_pos = event.localPos()
            self.can_move_scene = True
        else:
            super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        super().mouseMoveEvent(event)
        if self.can_move_scene:
            delta = (event.localPos() - self.old_pos) / self.translate_scalings
            self.old_pos = event.localPos()
            self.translate(delta.x(), delta.y())

    def mouseReleaseEvent(self, event):
        super().mouseReleaseEvent(event)
        self.can_move_scene = False

    @pyqtSlot('qreal')
    def scalingTime(self, t):
        factor = 1.0 + self.scheduled_scalings / 300.0
        self.translate_scalings *= 1.0 + self.scheduled_scalings / 300.0
        old_pos = self.mapToScene(self.geometry().center())
        self.scale(factor, factor)
        new_pos = self.mapToScene(self.geometry().center())
        delta = new_pos - old_pos
        self.translate(delta.x(), delta.y())

    @pyqtSlot()
    def animFinished(self):
        if self.scheduled_scalings > 0:
            self.scheduled_scalings -= 1
        else:
            self.scheduled_scalings += 1
        self.sender().deleteLater()


class Preview(QGraphicsView):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle('Preview')
        self.setBackgroundBrush(QColor(66, 66, 66))

    @pyqtSlot()
    def show_preview(self):
        self.show()
        self.activateWindow()

    def mousePressEvent(self, event):
        pass

    def mouseDoubleClickEvent(self, event):
        pass

    def resizeEvent(self, event):
        self.fitInView(self.sceneRect(), Qt.KeepAspectRatio)
