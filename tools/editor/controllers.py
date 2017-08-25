from PyQt5.QtCore import (
    QObject
)


class Controller(QObject):

    def __init__(self, scene, document, parent=None):
        super().__init__(parent)
        self.scene = scene
        self.document = document

    def add_object(self, pos):
        pass

    def remove_object(self, pos):
        pass

    def merge(self):
        pass