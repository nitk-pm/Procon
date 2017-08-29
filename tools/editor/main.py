import sys
from PyQt5.QtCore import (
    pyqtSlot,
    Qt
)
from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QAction,
    QMessageBox,
    QFileDialog
)
from PyQt5 import uic
from widgets import View, Preview, BoardScene
from models import Document, LayerDelegate, NodeDelegate


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('mainwindow.ui')[0]
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
            msg_box.setStandardButtons(QMessageBox.Save | QMessageBox.Cancel)
            msg_box.setDefaultButton(QMessageBox.Save)
            msg = msg_box.exec()
            if msg == QMessageBox.Save:
                self.save()

        self.document = Document()
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
        self.document.save_to_file(filename)

    @pyqtSlot()
    def load(self):
        filename = QFileDialog.getOpenFileName(
            self,
            'Load to file',
            '',
            'json (*.json)'
        )[0]
        self.document.load_to_file(filename)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
