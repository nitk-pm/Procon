from PyQt5.QtCore import (
    QObject
)
from graph import Graph


class Document(QObject):

    def __init__(self, parent):
        super().__init__(parent)
        self.graph = Graph()

    def add_vertex(self, pos):
        self.graph.add_node(pos)

    def remove_vertex(self, pos):
        self.graph.remove_node(pos)

    def add_edge(self, src, dest):
        self.graph.add_edge(src, dest)

    def remove_edge(self, src, dest):
        self.graph.remove_edge(src, dest)
