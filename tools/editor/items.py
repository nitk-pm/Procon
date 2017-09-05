from PyQt5.QtCore import (
    Qt,
    QPointF,
    QRectF,
    QLineF,
    QSizeF
)
from PyQt5.QtWidgets import (
    QGraphicsItem,
    QGraphicsEllipseItem,
    QGraphicsLineItem,
    QGraphicsPixmapItem,
    QUndoCommand
)
from PyQt5.QtGui import (
    QColor,
    QPen
)
from util import convert_from_str


class RestoreCommand(QUndoCommand):

    def __init__(self, document, before, after, parent=None):
        super().__init__(parent)
        self.document = document
        self.before = before
        self.after = after
        self.run = False

    def undo(self):
        self.document.restore(self.before)
        self.run = True

    def redo(self):
        if self.run:
            self.document.restore(self.after)


class Board(QGraphicsPixmapItem):

    def __init__(self, width=101, height=65, base=40, parent=None):
        super().__init__(parent)
        self.base = base
        self.radius = base / 10
        self.center = QPointF(base / 2, base / 2)
        self.grid_size = QSizeF(width, height)
        self.image_size = QSizeF(width * base, height * base)

        from PyQt5.QtGui import QImage, QPixmap, QPainter
        from itertools import product
        pixmap = QPixmap(self.image_size.width(), self.image_size.height())
        pixmap.fill()

        painter = QPainter()
        painter.begin(pixmap)
        painter.setBrush(Qt.black)

        for y, x in product(range(height), range(width)):
            offset = self.center + QPointF(base * x, base * y)
            painter.drawEllipse(offset, self.radius, self.radius)

        painter.end()

        self.setPixmap(pixmap)
        self.area = QRectF(
            0,
            0,
            self.image_size.width(),
            self.image_size.height()
        )

    def map_to_grid(self, pos):
        p = self.mapFromScene(pos) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def map_from_grid(self, pos):
        return self.mapToScene(pos * self.base) + self.center

    def snap_to_grid(self, pos):
        p = self.map_to_grid(pos)
        return self.map_from_grid(p)

    def contains(self, pos):
        return self.area.contains(pos)


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

    def remove_all(self):
        items = self.childItems()
        for item in items:
            self.remove_item(item)

    def paint(self, painter, option, widget=None):
        pass

    def boundingRect(self):
        return QRectF(0, 0, 0, 0)


class Edge(QGraphicsLineItem):

    def __init__(self, src_node, dest_node, document, parent=None):
        super().__init__(parent)
        self.document = document
        self.width = document.board.radius
        self.setPen(QPen(QColor('#000000'), self.width, cap=Qt.RoundCap))
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
        if old_node is self.source:
            self.source.edges.remove(self)
            self.source = new_node
            self.source.edges.append(self)
        elif old_node is self.dest:
            self.dest.edges.remove(self)
            self.dest = new_node
            self.dest.edges.append(self)

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
        Edge(node, self.dest, self.document, self.parentItem())
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

    def __init__(self, pos, document, parent=None):
        super().__init__(parent)
        self.edges = []
        self.document = document
        self.radius = document.board.radius

        self.setAcceptHoverEvents(True)
        self.setFlag(QGraphicsItem.ItemIsMovable)
        self.setFlag(QGraphicsItem.ItemIsSelectable)
        self.setFlag(QGraphicsItem.ItemSendsGeometryChanges)
        self.setCacheMode(QGraphicsItem.DeviceCoordinateCache)
        self.setPos(pos)
        self.setRect(QRectF(
            -self.radius * 2,
            -self.radius * 2,
            self.radius * 4,
            self.radius * 4
        ))

        self.normal_pen = QPen(QColor('#000000'), self.radius / 2, cap=Qt.RoundCap)
        self.enter_pen = QPen(QColor('#0288D1'), self.radius / 2, cap=Qt.RoundCap)
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
        if not self.document.is_created:
            self.document.hash.append(self.to_str())

    def grid_pos(self):
        return self.document.board.map_to_grid(self.pos())

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

    def boundingRect(self):
        r = self.document.board.base / 2 - 1
        return QRectF(-r, -r, r * 2, r * 2)

    def shape(self):
        from PyQt5.QtGui import QPainterPath
        path = QPainterPath()
        r = self.document.board.base / 2 - 1
        path.addEllipse(QRectF(-r, -r, r * 2, r * 2))
        return path

    def itemChange(self, change, value):
        if self.scene() is not None:
            if change == QGraphicsItem.ItemPositionHasChanged:
                for edge in self.edges:
                    edge.adjust()

            elif change == QGraphicsItem.ItemPositionChange:
                pos = self.document.board.snap_to_grid(value)
                if self.document.board.contains(pos):
                    return pos
                else:
                    return self.pos()

        return super().itemChange(change, value)

    def hoverEnterEvent(self, event):
        self.setPen(self.enter_pen)
        p = self.grid_pos()
        self.setToolTip('{0:.0f}, {1:.0f}'.format(p.x(), p.y()))
        super().hoverEnterEvent(event)

    def hoverLeaveEvent(self, event):
        self.setPen(self.normal_pen)
        super().hoverLeaveEvent(event)
