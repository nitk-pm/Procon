from PyQt5.QtCore import (
    Qt,
    QObject,
    QPointF,
    QRectF,
    QLineF,
    pyqtSignal,
    QAbstractItemModel,
    QModelIndex
)
from PyQt5.QtWidgets import (
    QGraphicsItem,
    QGraphicsEllipseItem,
    QGraphicsLineItem,
    QStyledItemDelegate,
    QDoubleSpinBox,
    QUndoStack
)
from PyQt5.QtGui import (
    QColor,
    QPen
)


class Layer(QGraphicsItem):

    def __init__(self, name='layer', z_value=0, opacity=1.0, parent=None):
        super().__init__(parent)
        self.name = name
        self.setZValue(z_value)
        self.setOpacity(opacity)

    def add_item(self, item):
        if item not in self.childItems():
            item.setParentItem(self)
            self.scene().update()

    def remove_item(self, item):
        if item in self.childItems():
            item.setParentItem(None)
            self.scene().removeItem(item)

    def paint(self, painter, option, widget=None):
        pass

    def boundingRect(self):
        return QRectF(0, 0, 0, 0)


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


class LayerDelegate(QStyledItemDelegate):

    def __init__(self, parent=None):
        super().__init__(parent)

    def createEditor(self, parent, option, index):
        if index.model().headers[index.column()] == 'opacity':
            spinbox = QDoubleSpinBox(parent)
            spinbox.setRange(0.0, 1.0)
            spinbox.setSingleStep(0.1)
            return spinbox
        return None

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.DisplayRole)
        if isinstance(editor, QDoubleSpinBox):
            editor.setValue(value)

    def setModelData(self, editor, model, index):
        model.setData(index, editor.value())


class Edge(QGraphicsLineItem):

    def __init__(self, src_node, dest_node, width, parent=None):
        super().__init__(parent)
        self.setPen(QPen(QColor('#000000'), width, cap=Qt.RoundCap))
        self.source = src_node
        self.dest = dest_node
        self.source.edges.append(self)
        self.dest.edges.append(self)
        self.adjust()

    def set_source(self, node):
        self.source = node
        self.adjust()

    def set_dest(self, node):
        self.dest = node
        self.adjust()

    def replace(self, old_node, new_node):
        if old_node == self.source:
            self.source.edges.remove(self)
            self.source = new_node
        elif old_node == self.dest:
            self.dest.edges.remove(self)
            self.dest = new_node

    def remove(self):
        if self.parentItem() is not None:
            self.parentItem().remove_item(self)
            self.source.edges.remove(self)
            self.dest.edges.remove(self)

    def adjust(self):
        from PyQt5.QtCore import QLineF
        self.setLine(QLineF(self.source.pos(), self.dest.pos()))

    def split(self, node):
        self.dest.edges.remove(self)
        node.edges.append(self)
        Edge(node, self.dest, 4, self.parentItem())
        self.dest = node
        self.adjust()

    def equal(self, edge):
        p1, p2 = self.line().p1(), self.line().p2()
        p3, p4 = edge.line().p1(), edge.line().p2()
        if p1 == p3 and p2 == p4:
            return True
        elif p1 == p4 and p2 == p3:
            return True
        return False


class Node(QGraphicsEllipseItem):

    def __init__(self, pos, radius, parent=None):
        super().__init__(parent)
        self.edges = []
        self.radius = radius

        self.setAcceptHoverEvents(True)
        self.setFlag(QGraphicsItem.ItemIsMovable)
        self.setFlag(QGraphicsItem.ItemIsSelectable)
        self.setFlag(QGraphicsItem.ItemSendsGeometryChanges)
        self.setCacheMode(QGraphicsItem.DeviceCoordinateCache)
        self.setPos(pos)
        self.setRect(QRectF(-radius * 2, -radius * 2, radius * 4, radius * 4))

        self.normal_pen = QPen(QColor('#000000'), radius / 2, cap=Qt.RoundCap)
        self.enter_pen = QPen(QColor('#0288D1'), radius / 2, cap=Qt.RoundCap)
        self.setPen(self.normal_pen)
        self.setBrush(QColor('#FFFFFF'))

    def remove(self):
        if self.scene() is None:
            return
        while len(self.edges) != 0:
            self.edges[0].remove()
        self.parentItem().remove_item(self)

    def merge(self):
        if self.scene() is None:
            return
        for item in self.scene().collidingItems(self):
            if isinstance(item, Node):
                while len(item.edges) != 0:
                    self.edges.append(item.edges[0])
                    item.edges[0].replace(item, self)
                item.remove()
            elif isinstance(item, Edge) and item not in self.edges:
                item.split(self)
        from itertools import product
        remove_list = {}
        length = len(self.edges)
        for i, j in product(range(length), range(length)):
            if i != j and i not in remove_list and j not in remove_list:
                if self.edges[i].equal(self.edges[j]):
                    remove_list[i] = self.edges[i]
        
        for edge in remove_list.values():
            edge.remove()

    def grid_pos(self):
        return self.scene().board.map_to_grid(self.pos())

    def to_str(self):
        p = self.grid_pos()
        return '({:.0f}, {:.0f})'.format(p.x(), p.y())

    def linked_nodes(self):
        nodes = []
        for edge in self.edges:
            if edge.source is self:
                nodes.append(edge.dest)
            else:
                nodes.append(edge.source)
        return nodes

    def itemChange(self, change, value):
        if self.scene() is not None:
            if change == QGraphicsItem.ItemPositionHasChanged:
                pos = self.scene().board.snap_to_grid(value)
                self.setPos(pos)
                for edge in self.edges:
                    edge.adjust()

            elif change == QGraphicsItem.ItemPositionChange:
                pos = self.scene().board.snap_to_grid(value)
                if not self.scene().board.contains(pos):
                    return self.pos()

        return super().itemChange(change, value)

    def boundingRect(self):
        r = self.radius
        return QRectF(-r * 4, -r * 4, r * 8, r * 8)

    def hoverEnterEvent(self, event):
        self.setPen(self.enter_pen)
        p = self.scene().board.map_to_grid(event.scenePos())
        self.setToolTip('{0:.0f}, {1:.0f}'.format(p.x(), p.y()))
        super().hoverEnterEvent(event)

    def hoverLeaveEvent(self, event):
        self.setPen(self.normal_pen)
        super().hoverLeaveEvent(event)


class NodeModel(QAbstractItemModel):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.headers = 'pos', 'linked nodes'
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
                if self.headers[index.column()] == 'pos':
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


class NodeDelegate(QStyledItemDelegate):

    def __init__(self, parent=None):
        super().__init__(parent)

    def createEditor(self, parent, option, index):
        return None

    def setEditorData(self, editor, index):
        pass

    def setModelData(self, editor, model, index):
        pass


class Document(QObject):

    def __init__(self, project_name='new document', parent=None):
        super().__init__(parent)
        self.full_path = ''
        self.dir_path = ''
        self.project_name = project_name
        self.history = QUndoStack(self)
        self.node_layer = Layer(name='node', z_value=1, opacity=1.0)
        self.edge_layer = Layer(name='edge', z_value=0, opacity=1.0)

        self.node_model = NodeModel(self)
        self.layer_model = LayerModel(self)
        self.layer_model.layers.append(self.node_layer)
        self.layer_model.layers.append(self.edge_layer)

    def create_node(self, pos):
        self.source = Node(pos, 4, self.node_layer)
        self.dest = Node(pos, 4, self.node_layer)
        self.edge = Edge(self.source, self.dest, 4, self.edge_layer)

    def merge_nodes(self, nodes=None):
        if nodes is None:
            if self.source.pos() == self.dest.pos():
                self.dest.remove()
            else:
                self.dest.merge()
            self.source.merge()
        else:
            for node in nodes:
                node.merge()
        self.update_models()

    def remove_nodes(self, nodes):
        for node in nodes:
            node.remove()
        self.update_models()

    def remove_all(self):
        for node in self.node_layer.childItems():
            node.remove()
        self.update_models()

    def to_dict(self):
        items = []
        for node in self.node_layer.childItems():
            items.append({
                'pos': node.to_str(),
                'linked_nodes': [n.to_str() for n in node.linked_nodes()]
            })
        project_data = {
            'name': self.project_name,
            'items': items
        }
        s = self.node_layer.scene().board.map_from_str(items[0]['pos'])
        return project_data

    def restore(self, project_data):
        self.remove_all()
        self.project_name = project_data['name']
        board = self.node_layer.scene().board
        for item in project_data['items']:
            pos = board.map_from_str(item['pos'])
            pos = board.map_from_grid(pos)
            source = Node(pos, 4, self.node_layer)
            for p_str in item['linked_nodes']:
                p = board.map_from_str(p_str)
                p = board.map_from_grid(p)
                dest = Node(p, 4, self.node_layer)
                Edge(source, dest, 4, self.edge_layer)
        self.merge_nodes(self.node_layer.childItems())

    def update_models(self):
        self.node_model.data_changed(self.node_layer.childItems())
        self.layer_model.data_changed()

    def save_to_file(self, filename):
        import os
        self.full_path = filename
        self.dir_path, self.project_name = os.path.split(filename)
        self.project_name = self.project_name.rsplit('.', 1)[0]
        with open(filename, 'w') as file:
            import json
            json.dump(self.to_dict(), file, indent=2)

    def load_to_file(self, filename):
        import os
        self.full_path = filename
        self.dir_path, self.project_name = os.path.split(filename)
        self.project_name = self.project_name.rsplit('.', 1)[0]
        with open(filename, 'r') as file:
            import json
            self.restore(json.load(file))