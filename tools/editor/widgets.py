from PyQt5.QtCore import (
    Qt,
    QObject,
    QTimer,
    QTimeLine,
    pyqtSlot,
    QEvent
)
from PyQt5.QtWidgets import (
    QGraphicsView,
    QGraphicsScene,
    QGraphicsSceneMouseEvent,
    QAction,
    QActionGroup
)
from PyQt5.QtGui import (
    QWheelEvent,
    QMouseEvent,
    QKeyEvent,
    QColor
)
from controllers import Controller


class BoardScene(QGraphicsScene):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.actions = QActionGroup(self)
        self.actions.setExclusive(True)
        self.selectionChanged.connect(self.select_controll)

    def setup_actions(self, piece: QAction, frame: QAction, delete: QAction):
        self.actions.addAction(piece)
        self.actions.addAction(frame)
        self.action_delete = delete
        self.action_delete.triggered.connect(self.delete_objects)

    def keyPressEvent(self, event: QKeyEvent):
        super().keyPressEvent(event)
        if event.key() == Qt.Key_Escape:
            Controller().interrupt()

    def mousePressEvent(self, event: QGraphicsSceneMouseEvent):
        if not Controller().document.is_editing:
            super().mousePressEvent(event)
        Controller().plot_vertex(event.scenePos())

    def mouseDoubleClickEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseDoubleClickEvent(event)
        Controller().create_object(
            event.scenePos(),
            self.actions.checkedAction().text()
        )

    def mouseMoveEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseMoveEvent(event)
        Controller().update_guide(event.scenePos())

    def mouseReleaseEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseReleaseEvent(event)

    @pyqtSlot()
    def select_controll(self):
        self.action_delete.setEnabled(len(self.selectedItems()) != 0)

    @pyqtSlot()
    def delete_objects(self):
        Controller().delete_objects(self.selectedItems())


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

    def wheelEvent(self, event: QWheelEvent):
        if event.modifiers() & Qt.ControlModifier:
            self.zoom(event.angleDelta().y())
        elif event.modifiers() & Qt.ShiftModifier:
            v = self.horizontalScrollBar().value() - event.angleDelta().y()
            self.horizontalScrollBar().setValue(v)
        else:
            super().wheelEvent(event)

    def mousePressEvent(self, event: QMouseEvent):
        if event.button() == Qt.RightButton:
            self.old_pos = event.localPos()
            self.can_move_scene = True
        else:
            super().mousePressEvent(event)

    def mouseMoveEvent(self, event: QMouseEvent):
        super().mouseMoveEvent(event)
        if self.can_move_scene:
            delta = (event.localPos() - self.old_pos) / self.translate_scalings
            self.old_pos = event.localPos()
            self.translate(delta.x(), delta.y())

    def mouseReleaseEvent(self, event: QMouseEvent):
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
