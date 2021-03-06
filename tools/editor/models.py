from PyQt5.QtCore import (
    Qt,
    QObject,
    pyqtSlot,
    QAbstractItemModel,
    QModelIndex
)
from PyQt5.QtWidgets import (
    QUndoStack
)
from items import (
    RestoreCommand,
    Board,
    Layer,
    Edge,
    Node
)
from util import convert_from_str, DefaultFormat
from logging import getLogger
logger = getLogger()


class LayerModel(QAbstractItemModel):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.headers = 'name', 'number', 'opacity'
        self.layers = []

    def index(self, row, column, parent=QModelIndex()):
        return self.createIndex(row, column, None)

    def parent(self, child):
        return QModelIndex()

    def rowCount(self, parent=QModelIndex()):
        return len(self.layers)

    def columnCount(self, parent=QModelIndex()):
        return len(self.headers)

    def data(self, index, role=Qt.DisplayRole):
        if role == Qt.DisplayRole:
            try:
                if self.headers[index.column()] == 'name':
                    return self.layers[index.row()].name
                elif self.headers[index.column()] == 'opacity':
                    return self.layers[index.row()].opacity()
                elif self.headers[index.column()] == 'number':
                    return len(self.layers[index.row()].childItems())
            except:
                return
        return

    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return
        if orientation == Qt.Horizontal:
            return self.headers[section]

    def flags(self, index):
        return super().flags(index) | Qt.ItemIsEditable

    def setData(self, index, value, role=Qt.EditRole):
        if role == Qt.EditRole:
            if self.headers[index.column()] == 'opacity':
                self.layers[index.row()].setOpacity(value)
                return True
        return False

    def data_changed(self):
        top_left = self.createIndex(0, 0)
        bottom_right = self.createIndex(len(self.layers), len(self.headers))
        self.dataChanged.emit(top_left, bottom_right)

    def update_layers(self, layers):
        self.beginResetModel()
        self.layers.clear()
        for layer in layers:
            self.layers.append(layer)
        self.endResetModel()


class NodeModel(QAbstractItemModel):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.headers = 'node', 'linked nodes'
        self.nodes = []

    def index(self, row, column, parent=QModelIndex()):
        return self.createIndex(row, column, None)

    def parent(self, child):
        return QModelIndex()

    def rowCount(self, parent=QModelIndex()):
        return len(self.nodes)

    def columnCount(self, parent=QModelIndex()):
        return len(self.headers)

    def data(self, index, role=Qt.DisplayRole):
        if role == Qt.DisplayRole:
            try:
                if self.headers[index.column()] == 'node':
                    return self.nodes[index.row()].to_str()
                elif self.headers[index.column()] == 'linked nodes':
                    nodes = self.nodes[index.row()].linked_nodes()
                    nodes = [node.to_str() for node in nodes]
                    return ', '.join(nodes)
            except:
                return
        if role == Qt.ToolTipRole:
            try:
                if self.headers[index.column()] == 'linked nodes':
                    nodes = self.nodes[index.row()].linked_nodes()
                    nodes = [node.to_str() for node in nodes]
                    return ', '.join(nodes)
            except:
                return
        return

    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return
        if orientation == Qt.Horizontal:
            return self.headers[section]

    def flags(self, index):
        return super().flags(index) | Qt.ItemIsEditable

    def data_changed(self, nodes):
        self.beginResetModel()
        self.nodes = nodes
        self.endResetModel()


class Document(QObject):

    def __init__(self, project_name='new document', parent=None):
        super().__init__(parent)
        self.full_path = ''
        self.dir_path = ''
        self.project_name = project_name
        self.history = QUndoStack(self)
        self.board = Board()
        self.node_layer = Layer(name='node', z_value=1, opacity=1.0)
        self.edge_layer = Layer(name='edge', z_value=0, opacity=1.0)

        self.node_model = NodeModel(self)
        self.layer_model = LayerModel(self)
        self.layer_model.layers.append(self.node_layer)
        self.layer_model.layers.append(self.edge_layer)

        self.is_created = False
        self.is_edit = False
        self.hash = []

    def connect_history(self, undo, redo, widget):
        undo.triggered.connect(self.history.undo)
        self.history.canUndoChanged.connect(undo.setEnabled)
        self.history.canUndoChanged.connect(widget.changed_edit_state)
        redo.triggered.connect(self.history.redo)
        self.history.canRedoChanged.connect(redo.setEnabled)

    def disconnect_history(self, undo, redo, widget):
        self.history.clear()
        undo.triggered.disconnect(self.history.undo)
        self.history.canUndoChanged.disconnect(undo.setEnabled)
        self.history.canUndoChanged.disconnect(widget.changed_edit_state)
        redo.triggered.disconnect(self.history.redo)
        self.history.canRedoChanged.disconnect(redo.setEnabled)

    def create_node(self, pos):
        self.dest = Node(pos, self, self.node_layer)
        self.source = Node(pos, self, self.node_layer)
        Edge(self.source, self.dest, self, self.edge_layer)
        self.source.setSelected(True)
        self.is_created = True

    def merge_nodes(self):
        if self.is_created:
            if self.source.pos() == self.dest.pos():
                self.dest.remove()
            else:
                self.dest.merge()
            self.source.merge()
            self.is_created = False
        else:
            self.hash.clear()
            for node in self.node_layer.childItems():
                if node.to_str() not in self.hash:
                    node.merge()
        self.update_models()

    def remove_nodes(self, nodes):
        self.begin_record()
        for node in nodes:
            node.remove()
        self.end_record()
        self.update_models()

    def remove_all(self):
        for node in self.node_layer.childItems():
            node.remove()
        self.update_models()

    def to_dict(self):
        items = []
        for node in self.node_layer.childItems():
            items.append({
                'node': node.to_str(),
                'linked-nodes': [n.to_str() for n in node.linked_nodes()]
            })
        project_data = {
            'name': self.project_name,
            'items': items
        }
        return project_data

    def restore(self, project_data):
        self.remove_all()
        self.project_name = project_data['name']
        for item in project_data['items']:
            pos = convert_from_str(item['node'])
            pos = self.board.map_from_grid(pos)
            source = Node(pos, self, self.node_layer)
            for p_str in item['linked-nodes']:
                p = convert_from_str(p_str)
                p = self.board.map_from_grid(p)
                dest = Node(p, self, self.node_layer)
                Edge(source, dest, self, self.edge_layer)
        self.merge_nodes()

    def update_models(self):
        self.node_model.data_changed(self.node_layer.childItems())
        self.layer_model.data_changed()

    def begin_record(self):
        self.before = self.to_dict()

    def end_record(self):
        after = self.to_dict()
        if not all((k, v) in self.before.items() for (k, v) in after.items()):
            self.history.push(RestoreCommand(self, self.before, after))

    def set_project_settings(self, filename):
        import os
        self.full_path = filename
        self.dir_path, self.project_name = os.path.split(filename)
        self.project_name = self.project_name.rsplit('.', 1)[0]

    def save(self, filename, format=DefaultFormat()):
        format.setup(self)
        format.save(filename)

    def load(self, filename, format=DefaultFormat()):
        format.setup(self)
        format.load(filename)
