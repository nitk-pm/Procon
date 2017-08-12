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
from ui_mainwindow import Ui_MainWindow
from widgets import View, Preview, BoardScene
from objects import Document, Board
from controllers import Controller


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        Controller().status = self.ui.statusBar

        scene = BoardScene()
        scene.setup_actions(
            self.ui.action_piece,
            self.ui.action_frame,
            self.ui.action_delete
        )

        Controller().document = Document()
        Controller().document.setup_to_scene(scene)
        Controller().document.setup_actions(
            self.ui.action_undo,
            self.ui.action_redo 
        )

        self.preview = Preview()
        self.preview.setScene(scene)
        self.ui.view.setScene(scene)
        self.ui.action_preview.triggered.connect(self.preview.show_preview)
        self.ui.action_save.triggered.connect(self.save)

        self.ui.action_piece.setChecked(True)

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
