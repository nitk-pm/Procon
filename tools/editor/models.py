from PyQt5.QtCore import (
    QObject,
    pyqtSignal
)
from PyQt5.QtWidgets import (
    QUndoStack
)


class Document(QObject):

    def __init__(self, parent):
        super().__init__(parent)
        self.history = QUndoStack()
        self.node_layer = None
        self.edge_layer = None
        self.src = None
        self.dest = None
        self.edge = None

    def create_edge(self, pos):
        pass

    def merge(self):
        pass

    def remove_nodes(self, nodes):
        for node in nodes:
            node.remove()
