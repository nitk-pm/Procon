from PyQt5.QtCore import (
    Qt,
    QObject,
    QPointF,
    QLineF,
    QRectF,
    pyqtSignal,
    pyqtSlot
)
from PyQt5.QtWidgets import (
    QUndoStack,
    QAction,
    QGraphicsItem,
    QGraphicsLineItem,
    QGraphicsPolygonItem,
    QGraphicsPixmapItem,
    QGraphicsScene
)
from PyQt5.QtGui import (
    QPen,
    QBrush,
    QKeyEvent
)


class Layer(QGraphicsItem):

    def __init__(self, name='layer', z_value=0, opacity=1.0, parent=None):
        super().__init__(parent)
        self.name = name
        self.setZValue(z_value)
        self.setOpacity(opacity)

    def paint(self, painter, option, widget=None):
        pass

    def boundingRect(self):
        return QRectF(0, 0, 0, 0)

    def add_item(self, item: QGraphicsItem):
        if item in self.childItems():
            print('This item is already included in the layer.')
            return
        item.setParentItem(self)
        self.scene().update()

    def remove_item(self, item: QGraphicsItem):
        if item not in self.childItems():
            print('This item is not in the layer.')
            return
        if item.scene() is not None:
            item.setParentItem(None)
            item.scene().removeItem(item)


class Board(QGraphicsPixmapItem):

    def __init__(self, width=101, height=65, base=1, parent=None):
        super().__init__(parent)
        base *= 20
        self.base = base * 2
        self.width = (width + 1) * self.base
        self.height = (height + 1) * self.base

        from PyQt5.QtGui import QImage, QPixmap, QPainter
        pixmap = QPixmap(self.width, self.height)
        pixmap.fill()
        painter = QPainter()
        painter.begin(pixmap)
        painter.setBrush(Qt.black)
        for y in range(1, height + 1):
            for x in range(1, width + 1):
                painter.drawEllipse(
                    x * self.base,
                    y * self.base,
                    base / 4,
                    base / 4
                )
        painter.end()
        self.setPixmap(pixmap)
        self.area = QRectF(1, 1, self.width - 1, self.height - 1)

    def adjust_to_data(self, pos: QPointF):
        b = QPointF(self.base / 2, self.base / 2)
        p = self.mapFromScene(pos - b) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def adjust_to_grid(self, pos: QPointF):
        p = self.adjust_to_data(pos)
        p = self.mapToScene(p * self.base)
        return p + QPointF(self.base, self.base) * 1.075

    def contains(self, pos: QPointF):
        return self.area.contains(pos)


class Document(QObject):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.undo_stack = QUndoStack()
        self.board = Board()
        self.layers = {
            'frame': Layer(name='frame', z_value=1, opacity=0.6),
            'piece': Layer(name='piece', z_value=2, opacity=1.0),
            'polygon': Layer(name='polygon', z_value=3, opacity=0.6),
            'guide': Layer(name='guide', z_value=4, opacity=1.0)
        }
        self.objects = []
        self.editing_object = None
        self.is_editing = False
        self.exist_frame = False

    def setup_actions(self, undo: QAction, redo: QAction):
        undo.triggered.connect(self.undo_stack.undo)
        self.undo_stack.canUndoChanged.connect(undo.setEnabled)
        redo.triggered.connect(self.undo_stack.redo)
        self.undo_stack.canRedoChanged.connect(redo.setEnabled)

    def setup_to_scene(self, scene: QGraphicsScene):
        scene.addItem(self.board)
        for layer in self.layers.values():
            scene.addItem(layer)

    def set_editing_object(self, mode: str, obj=None):
        self.editing_object = obj
        if mode == 'start':
            self.is_editing = True
        elif mode == 'end':
            self.is_editing = False

    def add_object(self, obj):
        if obj in self.objects:
            print('This object is already included in the list.')
            return
        self.objects.append(obj)
        if obj.object_type == 'frame':
            self.exist_frame = True

    def remove_object(self, obj):
        if obj not in self.objects:
            print('This object is not in the list.')
            return
        self.objects.remove(obj)
        if obj.object_type == 'frame':
            self.exist_frame = False


class Properties(object):

    def __init__(self, object_type: str):
        width = 2.0
        filled_color = Qt.blue
        if object_type == 'frame':
            width = 4.0
            filled_color = Qt.black
        self.guide = QPen(Qt.red, width, cap=Qt.RoundCap)
        self.normal = QPen(Qt.black, width, cap=Qt.RoundCap)
        self.edit = QPen(Qt.green, width, cap=Qt.RoundCap)
        self.filled = QBrush(filled_color)


class Object(QGraphicsPolygonItem):

    def __init__(self, object_type: str, document: Document, parent=None):
        super().__init__(parent)
        self.properties = Properties(object_type=object_type)
        self.document = document
        self.object_type = object_type
        self.vertexes = []
        self.edges = []
        self.guide = QGraphicsLineItem()
        self.is_closed = False
        self.is_show_guide = False

        self.guide.setPen(self.properties.guide)
        self.setBrush(self.properties.filled)
        self.setPen(QPen(Qt.NoPen))
        self.setFlags(QGraphicsItem.ItemIsSelectable)

    def push(self, pos: QPointF):
        if len(self.vertexes) != 0:
            edge = QGraphicsLineItem(QLineF(self.vertexes[-1], pos))
            edge.setPen(self.properties.edit)
            self.edges.append(edge)
            self.document.layers[self.object_type].add_item(edge)
        else:
            self.set_guide_show(True)
        self.vertexes.append(pos)
        if len(self.vertexes) > 2 and self.vertexes[0] == self.vertexes[-1]:
            self.is_closed = True

    def pop(self):
        if len(self.edges) != 0:
            self.document.layers[self.object_type].remove_item(self.edges[-1])
            self.edges.pop()
        self.vertexes.pop()
        self.is_closed = False
        if len(self.vertexes) == 0:
            self.set_guide_show(False)

    def update_guide(self, pos: QPointF):
        start = self.vertexes[-1]
        if start == pos:
            return
        self.guide.setLine(QLineF(start, pos))

    def set_guide_show(self, enable: bool):
        if enable:
            if not self.is_show_guide:
                self.document.layers['guide'].add_item(self.guide)
        else:
            if self.is_show_guide:
                self.document.layers['guide'].remove_item(self.guide)
        self.is_show_guide = enable

    def set_edit_mode(self, state: bool):
        if not self.is_closed:
                return
        if not state:
            from PyQt5.QtGui import QPolygonF
            if self.object_type == 'piece':
                self.setPolygon(QPolygonF(self.vertexes))
            elif self.object_type == 'frame':
                poly = QPolygonF(self.document.board.boundingRect())
                self.setPolygon(poly.subtracted(QPolygonF(self.vertexes)))
            self.document.layers['polygon'].add_item(self)
            self.set_guide_show(False)
            for edge in self.edges:
                edge.setPen(self.properties.normal)
            self.document.add_object(self)
            self.document.set_editing_object('end')
        else:
            self.document.layers['polygon'].remove_item(self)
            self.set_guide_show(True)
            for edge in self.edges:
                edge.setPen(self.properties.edit)
            self.document.remove_object(self)
            self.document.set_editing_object('start', self)

    def set_interrupt(self, flag: bool):
        if flag:
            for edge in self.edges:
                self.document.layers[self.object_type].remove_item(edge)
            if self.is_closed:
                self.document.layers['polygon'].remove_item(self)
            else:
                self.set_guide_show(False)
        else:
            for edge in self.edges:
                self.document.layers[self.object_type].add_item(edge)
            if self.is_closed:
                self.document.layers['polygon'].add_item(self)
            else:
                self.set_guide_show(True)

    def serialize(self):
        length = len(self.vertexes) - 1
        data = []
        for i in range(length):
            data.append(self.document.board.adjust_to_data(self.vertexes[i]))
        offset = self.boundingRect().topLeft()
        offset = self.document.board.adjust_to_data(offset)
        serial = '%d ' % length
        for i in range(length):
            p = data[i] - offset
            serial += '%d %d ' % (p.x(), p.y())
        return serial.strip()
