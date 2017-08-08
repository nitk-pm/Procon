from PyQt5.QtCore import (
    QPointF
)
from objects import (
    Document,
    Board,
    Object
)


class Singleton(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]


class Controller(object, metaclass=Singleton):

    status = None
    document = None

    def create_object(self, pos: QPointF, object_type: str):
        if self.document.is_editing:
            return

        if object_type == 'frame' and self.document.exist_frame:
            return

        p = self.document.board.adjust_to_grid(pos)
        if not self.document.board.contains(p):
            return
        from commands import CreateObject
        self.document.undo_stack.push(CreateObject(p, object_type))

    def plot_vertex(self, pos: QPointF):
        if not self.document.is_editing:
            return
        p = self.document.board.adjust_to_grid(pos)
        if not self.document.board.contains(p):
            return

        from commands import PlotVertex
        self.document.undo_stack.push(PlotVertex(p))

    def update_guide(self, pos: QPointF):
        if not self.document.is_editing:
            return
        p = self.document.board.adjust_to_grid(pos)
        if not self.document.board.contains(p):
            return
        self.document.editing_object.update_guide(p)

    def interrupt(self):
        if self.document.is_editing:
            from commands import InterruptObject
            self.document.undo_stack.push(InterruptObject())

    def delete_objects(self, objects):
        if len(objects) == 0:
            return
        from commands import DeleteObjects
        self.document.undo_stack.push(DeleteObjects(objects))

    def save(self, filename: str):
        if len(self.document.objects) == 0:
            return

        if not self.document.exist_frame:
            return

        data = {'frame': [], 'piece': []}
        for obj in self.document.objects:
            data[obj.object_type].append(obj.serialize())

        serial = '%d:' % len(data['piece'])
        for piece in data['piece']:
            serial += piece + ':'
        serial += data['frame'][0]
        with open(filename, 'w') as f:
            f.write(serial)

    def show_message(self, message):
        if self.status is not None:
            self.status.showMessage(message)
