from PyQt5.QtCore import (
    Qt,
    QObject,
    QTimer,
    QTimeLine,
    pyqtSlot,
    QPointF,
    QRectF
)
from PyQt5.QtWidgets import (
    QGraphicsView,
    QGraphicsScene,
    QGraphicsSceneMouseEvent,
    QAction,
    QActionGroup,
    QGraphicsItem,
    QGraphicsPixmapItem,
    QGraphicsLineItem,
    QGraphicsEllipseItem
)
from PyQt5.QtGui import (
    QWheelEvent,
    QMouseEvent,
    QKeyEvent,
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


class Board(QGraphicsPixmapItem):

    def __init__(self, width=101, height=65, base=1, parent=None):
        super().__init__(parent)
        base *= 20
        self.base = base * 2
        self.width = (width + 1) * self.base
        self.height = (height + 1) * self.base

        from PyQt5.QtGui import QImage, QPixmap, QPainter
        from itertools import product
        pixmap = QPixmap(self.width, self.height)
        pixmap.fill()

        painter = QPainter()
        painter.begin(pixmap)
        painter.setBrush(Qt.black)

        b = base / 4
        for y, x in product(range(1, height + 1), range(1, width + 1)):
            painter.drawEllipse(x * self.base, y * self.base, b, b)

        painter.end()

        self.setPixmap(pixmap)
        self.area = QRectF(1, 1, self.width - 1, self.height - 1)

    def map_from_grid(self, pos):
        b = QPointF(self.base / 2, self.base / 2)
        p = self.mapFromScene(pos - b) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def map_to_grid(self, pos):
        p = self.mapToScene(pos * self.base)
        return p + QPointF(self.base, self.base) * 1.075

    def contains(self, pos):
        return self.area.contains(pos)


class Edge(QGraphicsLineItem):

    def __init__(self, src_node, dest_node, width, parent=None):
        super().__init__(parent)
        self.setPen(QPen(QColor('#000000'), width, cap=Qt.RoundCap))
        self.source = src_node
        self.dest = dest_node
        self.source.add_edge(self)
        self.dest.add_edge(self)
        self.adjust()

    def set_source(self, node):
        self.source = node
        self.adjust()

    def set_dest(self, node):
        self.dest = node
        self.adjust()

    def remove(self):
        self.source.edges.remove(self)
        self.dest.edges.remove(self)
        self.parentItem().remove_item(self)

    def adjust(self):
        from PyQt5.QtCore import QLineF
        self.setLine(QLineF(self.source.pos(), self.dest.pos()))


class Node(QGraphicsEllipseItem):

    def __init__(self, pos, radius, board, parent=None):
        super().__init__(parent)
        self.board = board
        self.edges = []
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

    def add_edge(self, edge):
        self.edges.append(edge)

    def remove(self):
        if self.scene() is None:
            return

        for edge in self.edges:
            edge.remove()
        self.parentItem().remove_item(self)

    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemPositionHasChanged:
            grid = self.board.map_from_grid(value)
            pos = self.board.map_to_grid(grid)
            self.setPos(pos)
            for edge in self.edges:
                edge.adjust()

        elif change == QGraphicsItem.ItemPositionChange:
            grid = self.board.map_from_grid(value)
            pos = self.board.map_to_grid(grid)
            if not self.board.contains(pos):
                return self.pos()

        return super().itemChange(change, value)


class BoardScene(QGraphicsScene):

    def __init__(self, controller, board, parent=None):
        super().__init__(parent)
        self.controller = controller
        self.board = board
        self.vertex_layer = Layer(name='vertex', z_value=1, opacity=1.0)
        self.edge_layer = Layer(name='edge', z_value=0, opacity=1.0)
        self.actions = QActionGroup(self)
        self.action_delete = None

        self.addItem(self.board)
        self.addItem(self.vertex_layer)
        self.addItem(self.edge_layer)

        self.actions.setExclusive(True)
        self.selectionChanged.connect(self.select_controll)

        self.src = None
        self.dest = None
        self.edge = None

    def setup_actions(self, actions):
        if 'mode' in actions:
            for action in actions['mode']:
                self.actions.addAction(action)
        if 'delete' in actions:
            self.action_delete = actions['delete']
            self.action_delete.triggered.connect(self.delete_objects)

    def keyPressEvent(self, event: QKeyEvent):
        super().keyPressEvent(event)

    def mousePressEvent(self, event: QGraphicsSceneMouseEvent):
        grid = self.board.map_from_grid(event.scenePos())
        pos = self.board.map_to_grid(grid)
        if self.actions.checkedAction().text() == 'edge':
            src = Node(pos, 4, self.board)
            dest = Node(pos, 4, self.board)
            edge = Edge(src, dest, 4)
            self.vertex_layer.add_item(src)
            self.vertex_layer.add_item(dest)
            self.edge_layer.add_item(edge)
            print(src.parentItem())
        elif self.actions.checkedAction().text() == 'select':
            pass
        super().mousePressEvent(event)

    def mouseDoubleClickEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseDoubleClickEvent(event)

    def mouseMoveEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseReleaseEvent(event)

    @pyqtSlot()
    def select_controll(self):
        self.action_delete.setEnabled(len(self.selectedItems()) != 0)

    @pyqtSlot()
    def delete_objects(self):
        for item in self.selectedItems():
            print(item)
            item.remove()


class View(QGraphicsView):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setBackgroundBrush(QColor(66, 66, 66))
        self.setMouseTracking(True)
        self.scheduled_scalings = 0
        self.translate_scalings = 1.0
        self.can_move_scene = False
        self.old_pos = None

    def zoom(self, delta):
        degrees = int(delta / 8)
        steps = int(degrees / 15)
        self.scheduled_scalings += steps
        if self.scheduled_scalings * steps < 0:
            self.scheduled_scalings = steps
        anim = QTimeLine(350, self)
        anim.setUpdateInterval(20)
        anim.valueChanged.connect(self.scalingTime)
        anim.finished.connect(self.animFinished)
        anim.start()

    def wheelEvent(self, event: QWheelEvent):
        if event.modifiers() & Qt.ControlModifier:
            self.zoom(event.angleDelta().y())
        elif event.modifiers() & Qt.ShiftModifier:
            v = self.horizontalScrollBar().value() - event.angleDelta().y()
            self.horizontalScrollBar().setValue(v)
        else:
            super().wheelEvent(event)

    def mousePressEvent(self, event: QMouseEvent):
        if event.button() == Qt.RightButton:
            self.old_pos = event.localPos()
            self.can_move_scene = True
        else:
            super().mousePressEvent(event)

    def mouseMoveEvent(self, event: QMouseEvent):
        super().mouseMoveEvent(event)
        if self.can_move_scene:
            delta = (event.localPos() - self.old_pos) / self.translate_scalings
            self.old_pos = event.localPos()
            self.translate(delta.x(), delta.y())

    def mouseReleaseEvent(self, event: QMouseEvent):
        super().mouseReleaseEvent(event)
        self.can_move_scene = False

    @pyqtSlot('qreal')
    def scalingTime(self, t):
        factor = 1.0 + self.scheduled_scalings / 300.0
        self.translate_scalings *= 1.0 + self.scheduled_scalings / 300.0
        old_pos = self.mapToScene(self.geometry().center())
        self.scale(factor, factor)
        new_pos = self.mapToScene(self.geometry().center())
        delta = new_pos - old_pos
        self.translate(delta.x(), delta.y())

    @pyqtSlot()
    def animFinished(self):
        if self.scheduled_scalings > 0:
            self.scheduled_scalings -= 1
        else:
            self.scheduled_scalings += 1
        self.sender().deleteLater()


class Preview(QGraphicsView):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle('Preview')
        self.setBackgroundBrush(QColor(66, 66, 66))

    @pyqtSlot()
    def show_preview(self):
        self.show()
        self.activateWindow()

    def mousePressEvent(self, event):
        pass

    def mouseDoubleClickEvent(self, event):
        pass

    def resizeEvent(self, event):
        self.fitInView(self.sceneRect(), Qt.KeepAspectRatio)
