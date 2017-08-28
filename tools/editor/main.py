import sys
from PyQt5.QtCore import (
    pyqtSlot,
    Qt
)
from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QAction
)
from PyQt5 import uic
from widgets import View, Preview, BoardScene, Board
from models import Document


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('mainwindow.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)

        self.document = Document()

        self.scene = BoardScene(self.document)
        self.scene.setup_actions({
            'mode': [
                self.ui.action_select,
                self.ui.action_edge
            ],
            'delete': self.ui.action_delete
        })
        self.ui.view.setScene(self.scene)

        self.preview = Preview()
        self.preview.setScene(self.scene)

        self.ui.action_preview.triggered.connect(self.preview.show_preview)
        self.ui.action_save.triggered.connect(self.save)
        self.ui.action_screenshot.triggered.connect(self.screenshot)

    def closeEvent(self, event):
        super().closeEvent(event)
        self.preview.deleteLater()

    @pyqtSlot()
    def screenshot(self):
        from PyQt5.QtWidgets import QFileDialog
        from PyQt5.QtGui import QPainter, QImage
        filename = QFileDialog.getSaveFileName(
            self,
            'save file',
            '',
            'jpg (*.jpg)'
        )
        w = self.scene.width()
        h = self.scene.height()
        image = QImage(w, h, QImage.Format_ARGB32)
        with QPainter(image) as painter:
            self.scene.render(painter)
            image.save(filename[0])

    @pyqtSlot()
    def save(self):
        from PyQt5.QtWidgets import QFileDialog
        filename = QFileDialog.getSaveFileName()
        Controller().save(filename[0])


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
