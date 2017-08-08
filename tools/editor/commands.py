from PyQt5.QtCore import QPointF
from PyQt5.QtWidgets import QUndoCommand
from objects import Document, Object
from controllers import Controller


class CreateObject(QUndoCommand):

    def __init__(self, pos: QPointF, object_type: str, parent=None):
        super().__init__(parent)
        self.pos = pos
        self.obj = Object(object_type, Controller().document)

    def undo(self):
        self.obj.pop()
        self.obj.set_guide_show(False)
        self.obj.document.set_editing_object('end')

    def redo(self):
        self.obj.push(self.pos)
        self.obj.set_guide_show(True)
        self.obj.document.set_editing_object('start', self.obj)


class PlotVertex(QUndoCommand):

    def __init__(self, pos: QPointF, parent=None):
        super().__init__(parent)
        self.obj = Controller().document.editing_object
        self.pos = pos

    def undo(self):
        self.obj.set_edit_mode(True)
        self.obj.pop()

    def redo(self):
        self.obj.push(self.pos)
        self.obj.set_edit_mode(False)
        if self.obj.is_closed:
            print(self.obj.serialize())


class InterruptObject(QUndoCommand):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.obj = Controller().document.editing_object

    def undo(self):
        self.obj.set_interrupt(False)
        self.obj.document.set_editing_object('start', self.obj)

    def redo(self):
        self.obj.set_interrupt(True)
        self.obj.document.set_editing_object('end')


class DeleteObjects(QUndoCommand):

    def __init__(self, objects, parent=None):
        super().__init__(parent)
        self.objects = objects

    def undo(self):
        for obj in self.objects:
            obj.set_interrupt(False)
            obj.document.add_object(obj)
        print('delete undo')

    def redo(self):
        for obj in self.objects:
            obj.set_interrupt(True)
            obj.document.remove_object(obj)
        print('delete redo')
