from PyQt5.QtCore import (
    QObject
)
from PyQt5.QtWidgets import (
    QGraphicsItem,
    QGraphicsPixmapItem,
    QUndoStack
)


class Document(QObject):

    def __init__(self, parent):
        super().__init__(parent)
        self.history = QUndoStack()
        self.vertexes = []
        self.edges = []

    def add_edge(self, src, dest):
        pass
