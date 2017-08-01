from PyQt5.QtCore import QPointF
from PyQt5.QtWidgets import QUndoCommand
from objects import Document, Object, Controller


class CreateObject(QUndoCommand):

    def __init__(self, pos: QPointF, obj_type: str, parent=None):
        super().__init__(parent)
        self.pos = pos
        self.obj = Object(obj_type=obj_type)

    def undo(self):
        self.obj.pop()
        Controller().document.removeObject(self.obj)

    def redo(self):
        self.obj.push(self.pos)
        Controller().document.addObject(self.obj)


class PlotVertex(QUndoCommand):

    def __init__(self, pos: QPointF, parent=None):
        super().__init__(parent)
        self.obj = Controller().document.currentObject()
        self.pos = pos

    def undo(self):
        self.obj.pop()
        if not Controller().document.isEditing():
            self.obj.enableGuideEdge()
            self.obj.resumeEditing()

    def redo(self):
        self.obj.push(self.pos)
        if not self.obj.isEditable():
            self.obj.disableGuideEdge()
            self.obj.editingEnd()
            print(self.obj.serialize())
