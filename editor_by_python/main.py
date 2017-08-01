import sys
from PyQt5.QtCore import (
    pyqtSlot
)
from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow
)
from ui_mainwindow import Ui_MainWindow
from widgets import View, Preview
from objects import Document, Controller, Board, BoardScene


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.document = Document()
        self.document.setupUndoStack(self.ui.action_undo, self.ui.action_redo)

        self.board = Board()

        Controller().setDocument(self.document)
        Controller().setBoard(self.board)

        scene = BoardScene()
        scene.addItem(self.board)
        for layer in self.document.layers.values():
            scene.addItem(layer)
        self.ui.action_frame.toggled.connect(scene.changeCreateMode)

        self.ui.view.setScene(scene)

        self.preview = Preview()
        self.preview.setScene(scene)
        self.ui.action_preview.triggered.connect(self.preview.showPreview)

        self.ui.action_save.triggered.connect(self.save)

    def closeEvent(self, event):
        super().closeEvent(event)
        self.preview.deleteLater()

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
