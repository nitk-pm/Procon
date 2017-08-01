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
    QGraphicsScene,
    QGraphicsSceneMouseEvent
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

    def addItem(self, item: QGraphicsItem):
        if item in self.childItems():
            print('This item is already included in the layer.')
            return
        item.setParentItem(self)
        self.scene().update()

    def removeItem(self, item: QGraphicsItem):
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

    def adjustToData(self, pos: QPointF):
        b = QPointF(self.base / 2, self.base / 2)
        p = self.mapFromScene(pos - b) / self.base
        return QPointF(int(p.x()), int(p.y()))

    def adjustToGrid(self, pos: QPointF):
        p = self.adjustToData(pos)
        p = self.mapToScene(p * self.base)
        return p + QPointF(self.base, self.base) * 1.075

    def contains(self, pos: QPointF):
        return self.area.contains(pos)


class Document(QObject):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.undo_stack = QUndoStack()
        self.objects = []
        self.layers = {
            'frame': Layer(name='frame', z_value=1, opacity=0.6),
            'piece': Layer(name='piece', z_value=2, opacity=1.0),
            'polygon': Layer(name='polygon', z_value=3, opacity=0.6),
            'guide': Layer(name='guide', z_value=4, opacity=1.0)
        }
        self.is_editing = False
        self.exist_frame = False

    def setupUndoStack(self, undo: QAction, redo: QAction):
        undo.triggered.connect(self.undo_stack.undo)
        self.undo_stack.canUndoChanged.connect(undo.setEnabled)
        redo.triggered.connect(self.undo_stack.redo)
        self.undo_stack.canRedoChanged.connect(redo.setEnabled)

    def currentObject(self):
        if len(self.objects) == 0:
            return None
        return self.objects[-1]

    def addObject(self, obj):
        if obj in self.objects:
            print('This object is already included in the list.')
            return
        self.objects.append(obj)
        if obj.obj_type == 'frame':
            self.exist_frame = True
        self.is_editing = True

    def removeObject(self, obj):
        if obj not in self.objects:
            print('This object is not in the list.')
            return
        self.objects.remove(obj)
        if obj.obj_type == 'frame':
            self.exist_frame = False
        self.is_editing = False

    def isEditing(self):
        return self.is_editing

    def doesFrameExist(self):
        return self.exist_frame


class Singleton(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class Controller(object, metaclass=Singleton):

    document = None
    board = None

    def setDocument(self, document: Document):
        self.document = document

    def setBoard(self, board: Board):
        self.board = board

    def createPiece(self, pos: QPointF):
        if self.document.isEditing():
            return
        self.createObject(pos=pos, obj_type='piece')

    def createFrame(self, pos: QPointF):
        if self.document.isEditing() or self.document.doesFrameExist():
            return
        self.createObject(pos=pos, obj_type='frame')

    def createObject(self, pos: QPointF, obj_type: str):
        from commands import CreateObject
        self.document.undo_stack.push(CreateObject(pos=pos, obj_type=obj_type))

    def plotVertex(self, pos: QPointF):
        if not self.document.isEditing():
            return

        from commands import PlotVertex
        self.document.undo_stack.push(PlotVertex(pos=pos))

    def updateCurrentObject(self, pos: QPointF):
        if not self.document.isEditing():
            return
        self.document.currentObject().updateGuideEdge(pos)

    def interrupt(self):
        if self.document.isEditing():
            from commands import InterruptObject
            self.document.undo_stack.push(InterruptObject())

    def save(self, filename: str):
        if len(self.document.objects) == 0:
            return

        if not self.document.doesFrameExist():
            return

        data = {'frame': [], 'piece': []}
        for obj in self.document.objects:
            data[obj.obj_type].append(obj.serialize())

        serial = '%d:' % len(data['piece'])
        for piece in data['piece']:
            serial += piece + ':'
        serial += data['frame'][0]
        with open(filename, 'w') as f:
            f.write(serial)


class Properties(object):

    def __init__(self, obj_type: str):
        width = 2.0
        style = Qt.SolidPattern
        poly = Qt.blue
        if obj_type == 'frame':
            width *= 4
            poly = Qt.black
        self.guide = QPen(Qt.red, width, cap=Qt.RoundCap)
        self.normal = QPen(Qt.black, width, cap=Qt.RoundCap)
        self.edit = QPen(Qt.green, width, cap=Qt.RoundCap)
        self.polygon = QBrush(poly, style=style)


class Object(QGraphicsPolygonItem):

    def __init__(self, obj_type: str, parent=None):
        super().__init__(parent)
        self.properties = Properties(obj_type=obj_type)
        self.obj_type = obj_type
        self.vertexes = []
        self.edge_items = []
        self.guide_edge_item = QGraphicsLineItem()
        self.guide_edge_item.setPen(self.properties.guide)
        self.setBrush(self.properties.polygon)
        pen = QPen()
        pen.setBrush(QBrush(Qt.black, style=Qt.NoBrush))
        self.setPen(pen)
        if self.obj_type == 'piece':
            self.setFlags(QGraphicsItem.ItemIsSelectable)

    def push(self, pos: QPointF):
        if len(self.vertexes) != 0:
            edge = QGraphicsLineItem(QLineF(self.vertexes[-1], pos))
            edge.setPen(self.properties.edit)
            self.edge_items.append(edge)
            Controller().document.layers[self.obj_type].addItem(edge)
        else:
            self.enableGuideEdge()
        self.vertexes.append(pos)

    def pop(self):
        if len(self.edge_items) != 0:
            Controller().document.layers[self.obj_type].removeItem(
                self.edge_items[-1]
            )
            self.edge_items.pop()
        self.vertexes.pop()
        if len(self.vertexes) == 0:
            self.disableGuideEdge()

    def updateGuideEdge(self, pos: QPointF):
        start = self.vertexes[-1]
        if start == pos:
            return
        self.guide_edge_item.setLine(QLineF(start, pos))

    def editingEnd(self):
        from PyQt5.QtGui import QPolygonF
        if self.obj_type == 'piece':
            self.setPolygon(QPolygonF(self.vertexes))
        elif self.obj_type == 'frame':
            poly = QPolygonF(Controller().board.boundingRect())
            self.setPolygon(poly.subtracted(QPolygonF(self.vertexes)))

        Controller().document.layers['polygon'].addItem(self)
        for edge in self.edge_items:
            edge.setPen(self.properties.normal)
        Controller().document.is_editing = False

    def resumeEditing(self):
        Controller().document.layers['polygon'].removeItem(self)
        for edge in self.edge_items:
            edge.setPen(self.properties.edit)
        Controller().document.is_editing = True

    def isEditable(self):
        if len(self.edge_items) < 3:
            return True
        return self.vertexes[0] != self.vertexes[-1]

    def setInterrupt(self, flag: bool):
        if flag:
            for edge in self.edge_items:
                Controller().document.layers[self.obj_type].removeItem(edge)
            self.disableGuideEdge()
        else:
            for edge in self.edge_items:
                Controller().document.layers[self.obj_type].addItem(edge)
            self.enableGuideEdge()

    def enableGuideEdge(self):
        Controller().document.layers['guide'].addItem(self.guide_edge_item)

    def disableGuideEdge(self):
        Controller().document.layers['guide'].removeItem(self.guide_edge_item)

    def serialize(self):
        length = len(self.vertexes) - 1
        data = []
        for i in range(length):
            data.append(Controller().board.adjustToData(self.vertexes[i]))
        offset = self.boundingRect().topLeft()
        offset = Controller().board.adjustToData(offset)
        serial = '%d ' % length
        for i in range(length):
            p = data[i] - offset
            serial += '%d %d ' % (p.x(), p.y())
        return serial.strip()

    def itemChange(self, change, value):
        if change == QGraphicsItem.ItemSelectedChange:
            if Controller().document.isEditing():
                return None
        return super().itemChange(change, value)


class BoardScene(QGraphicsScene):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.create_frame = False

    @pyqtSlot(bool)
    def changeCreateMode(self, flag: bool):
        self.create_frame = flag

    def keyPressEvent(self, event: QKeyEvent):
        super().keyPressEvent(event)
        if event.key() == Qt.Key_Escape:
            Controller().interrupt()

    def mousePressEvent(self, event: QGraphicsSceneMouseEvent):
        super().mousePressEvent(event)

        pos = Controller().board.adjustToGrid(event.scenePos())
        if Controller().board.contains(pos):
            Controller().plotVertex(pos)

    def mouseDoubleClickEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseDoubleClickEvent(event)

        pos = Controller().board.adjustToGrid(event.scenePos())
        if Controller().board.contains(pos):
            if self.create_frame:
                Controller().createFrame(pos)
            else:
                Controller().createPiece(pos)

    def mouseMoveEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseMoveEvent(event)
        pos = Controller().board.adjustToGrid(event.scenePos())
        if Controller().board.contains(pos):
            Controller().updateCurrentObject(pos)

    def mouseReleaseEvent(self, event: QGraphicsSceneMouseEvent):
        super().mouseReleaseEvent(event)
