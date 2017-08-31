from PyQt5.QtCore import (
    Qt,
    QObject,
    QTimer,
    QTimeLine,
    pyqtSlot,
    pyqtSignal,
)
from PyQt5.QtWidgets import (
    QGraphicsView,
    QGraphicsScene,
    QActionGroup,
    QStyledItemDelegate,
    QDoubleSpinBox,
    QMainWindow,
    QMessageBox,
    QFileDialog,
)
from PyQt5.QtGui import (
    QColor,
    QCursor
)
from PyQt5 import uic
from models import Document


class BoardScene(QGraphicsScene):

    show_message = pyqtSignal(str)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.actions = QActionGroup(self)
        self.action_edge = None
        self.action_select = None
        self.action_delete = None
        self.document = None

        self.actions.setExclusive(True)
        self.selectionChanged.connect(self.select_controll)

    def set_document(self, document):
        if self.document is not None:
            self.removeItem(self.document.board)
            self.removeItem(self.document.node_layer)
            self.removeItem(self.document.edge_layer)
        self.document = document
        self.addItem(self.document.board)
        self.addItem(self.document.node_layer)
        self.addItem(self.document.edge_layer)

    def set_actions(self, actions):
        if 'mode' in actions:
            for action in actions['mode']:
                self.actions.addAction(action)
        if 'delete' in actions:
            self.action_delete = actions['delete']
            self.action_delete.triggered.connect(self.delete_objects)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.clearSelection()
        if event.key() == Qt.Key_S:
            from util import ClosedGraphDetector
            detector = ClosedGraphDetector(self.document.to_dict())
            print('search result')
            for path in detector.search():
                print(path)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton and self.document is not None:
            self.grid_pos = self.document.board.map_to_grid(event.scenePos())
            self.document.begin_record()
            if self.actions.checkedAction().text() == 'edge':
                self.clearSelection()
                self.document.create_node(event.scenePos())
            elif self.actions.checkedAction().text() == 'select':
                pass
        super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if event.buttons() & Qt.LeftButton and len(self.selectedItems()) != 0:
            p = self.document.board.map_to_grid(event.scenePos())
            d = p - self.grid_pos
            self.show_message.emit(
                'pos: ({:.0f}, {:.0f})  delta: ({:.0f}, {:.0f})'
                .format(p.x(), p.y(), d.x(), d.y())
            )
        else:
            p = self.document.board.map_to_grid(event.scenePos())
            self.show_message.emit(
                'pos: ({:.0f}, {:.0f})'.format(p.x(), p.y())
            )
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.LeftButton and self.document is not None:
            if self.actions.checkedAction().text() == 'edge':
                self.document.merge_nodes()
                self.clearSelection()
            elif self.actions.checkedAction().text() == 'select':
                self.document.merge_nodes()
            self.document.end_record()
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


class LayerDelegate(QStyledItemDelegate):

    def __init__(self, parent=None):
        super().__init__(parent)

    def createEditor(self, parent, option, index):
        if index.model().headers[index.column()] == 'opacity':
            spinbox = QDoubleSpinBox(parent)
            spinbox.setRange(0.0, 1.0)
            spinbox.setSingleStep(0.1)
            return spinbox
        return None

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.DisplayRole)
        if isinstance(editor, QDoubleSpinBox):
            editor.setValue(value)

    def setModelData(self, editor, model, index):
        model.setData(index, editor.value())


class NodeDelegate(QStyledItemDelegate):

    def __init__(self, parent=None):
        super().__init__(parent)

    def createEditor(self, parent, option, index):
        return None

    def setEditorData(self, editor, index):
        pass

    def setModelData(self, editor, model, index):
        pass


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('form.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)

        self.ui.layer_view.setItemDelegate(LayerDelegate())
        self.ui.node_view.setItemDelegate(NodeDelegate())

        self.scene = BoardScene()
        self.scene.set_actions({
            'mode': [
                self.ui.action_select,
                self.ui.action_edge
            ],
            'delete': self.ui.action_delete
        })
        self.scene.show_message.connect(self.ui.status.showMessage)
        self.ui.view.setScene(self.scene)

        self.preview = Preview()
        self.preview.setScene(self.scene)

        self.ui.action_new.triggered.connect(self.new_document)
        self.ui.action_preview.triggered.connect(self.preview.show_preview)
        self.ui.action_screenshot.triggered.connect(self.screenshot)
        self.ui.action_open.triggered.connect(self.load)
        self.ui.action_save.triggered.connect(self.save)

        self.document = None
        self.new_document()

    def closeEvent(self, event):
        super().closeEvent(event)
        self.preview.deleteLater()

    @pyqtSlot()
    def new_document(self):
        if self.document is not None:
            msg_box = QMessageBox(self)
            msg_box.setText((
                'Do you want to save change you'
                'changes you made to Document ?'
            ))
            msg_box.setStandardButtons(QMessageBox.Save | QMessageBox.No | QMessageBox.Cancel)
            msg_box.setDefaultButton(QMessageBox.Save)
            msg = msg_box.exec()
            if msg == QMessageBox.Save:
                self.save()
            elif msg == QMessageBox.Cancel:
                return
            self.document.disconnect_actions(
                self.ui.action_undo,
                self.ui.action_redo
            )

        self.document = Document()
        self.document.connect_actions(self.ui.action_undo, self.ui.action_redo)
        self.scene.set_document(self.document)
        self.ui.layer_view.setModel(self.document.layer_model)
        self.ui.node_view.setModel(self.document.node_model)

    @pyqtSlot()
    def screenshot(self):
        from PyQt5.QtGui import QPainter, QImage
        filename = QFileDialog.getSaveFileName(
            self,
            'Save to screenshot',
            '',
            'png (*.png)'
        )
        if filename != '':
            w, h = self.scene.width(), self.scene.height()
            image = QImage(w, h, QImage.Format_ARGB32)
            with QPainter(image) as painter:
                self.scene.render(painter)
                image.save(filename[0])

    @pyqtSlot()
    def save(self):
        filename = QFileDialog.getSaveFileName(
            self,
            'Save to file',
            self.document.project_name,
            'json (*.json)'
        )[0]
        if filename != '':
            self.document.save_to_file(filename)

    @pyqtSlot()
    def load(self):
        filename = QFileDialog.getOpenFileName(
            self,
            'Load to file',
            '',
            'json (*.json)'
        )[0]
        if filename != '':
            self.document.load_to_file(filename)
