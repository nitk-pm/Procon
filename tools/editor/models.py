from PyQt5.QtCore import (
    Qt,
    QObject,
    QPointF,
    QRectF,
    pyqtSignal,
    QAbstractItemModel,
    QModelIndex
)
from PyQt5.QtWidgets import (
    QGraphicsItem,
    QGraphicsEllipseItem,
    QGraphicsLineItem
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
                elif self.headers[index.column()] == 'number':
                    return len(self.layers[index.row()].childItems())
                elif self.headers[index.column()] == 'opacity':
                    return self.layers[index.row()].opacity()
            except:
                return
        return

    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return
        if orientation == Qt.Horizontal:
            return self.headers[section]

    def data_changed(self):
        pass


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
        remove_list = []
        length = len(self.edges)
        for i in range(length - 1):
            if self.edges[i].equal(self.edges[i + 1]):
                remove_list.append(self.edges[i])
            elif self.edges[i].line().length() == 0:
                remove_list.append(self.edges[i])

        for edge in remove_list:
            edge.remove()

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
        super().hoverEnterEvent(event)

    def hoverLeaveEvent(self, event):
        self.setPen(self.normal_pen)
        super().hoverLeaveEvent(event)


class Document(QObject):

    def __init__(self, filename='new document', parent=None):
        super().__init__(parent)
        self.filename = filename
        self.node_layer = Layer(name='node', z_value=1, opacity=1.0)
        self.edge_layer = Layer(name='edge', z_value=0, opacity=1.0)

        self.layer_model = LayerModel(self)
        self.layer_model.layers.append(self.node_layer)
        self.layer_model.layers.append(self.edge_layer)

    def create_node(self, pos):
        self.source = Node(pos, 4, self.node_layer)
        self.dest = Node(pos, 4, self.node_layer)
        self.edge = Edge(self.source, self.dest, 4, self.edge_layer)
        self.data_changed()

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
        self.data_changed()

    def remove_nodes(self, nodes):
        for node in nodes:
            node.remove()
        self.data_changed()

    def save(self, filename):
        pass

    def load(self, filename):
        pass

    def data_changed(self):
        index = self.layer_model.createIndex(0, 0)
        index2 = self.layer_model.createIndex(1, 2)
        self.layer_model.dataChanged.emit(index, index2)