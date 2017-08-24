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
from objects import Document
from controllers import Controller


class MainWindow(QMainWindow):

    def __init__(self, parent=None):
        super().__init__(parent)
        Ui = uic.loadUiType('mainwindow.ui')[0]
        self.ui = Ui()
        self.ui.setupUi(self)

        # Controller().status = self.ui.statusBar

        scene = BoardScene(None, Board())
        scene.setup_actions({
            'mode': [
                self.ui.action_select,
                self.ui.action_edge
            ],
            'delete': self.ui.action_delete
        })

        # scene.setup_actions(
        #     self.ui.action_piece,
        #     self.ui.action_frame,
        #     self.ui.action_delete
        # )

        # Controller().document = Document()
        # Controller().document.setup_to_scene(scene)
        # Controller().document.setup_actions(
        #     self.ui.action_undo,
        #     self.ui.action_redo 
        # )

        self.preview = Preview()
        self.preview.setScene(scene)
        self.ui.view.setScene(scene)
        self.ui.action_preview.triggered.connect(self.preview.show_preview)
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
