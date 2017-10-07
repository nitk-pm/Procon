from PyQt5.QtCore import (
    QPointF,
    Qt
)
from PyQt5.QtWidgets import (
    QApplication,
    QWidget,
    QFileDialog,
    QStyleOption,
    QStyle
)
from PyQt5.QtGui import (
    QPainter,
    QPolygonF
)


class Visualiser(QWidget):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setStyleSheet('background-color: rgb(66, 66, 66);margin: 5px;')
        self.data = None
        self.filename = QFileDialog.getOpenFileName(
            self,
            'open file',
            '',
            'text (*.txt);;json (*.json)'
        )[0]

        if self.filename.rsplit('.', 1)[1] == 'txt':
            self.load_text()
        else:
            self.answerfile = QFileDialog.getOpenFileName(
                self,
                'open file',
                '',
                'json (*.json)'
            )[0]
            self.load_json()
        self.united_rect = self.data[0].boundingRect()
        for d in self.data:
            self.united_rect = self.united_rect.united(d.boundingRect())
            self.resize(
                self.united_rect.width() + 20,
                self.united_rect.height() + 20
            )
        self.piece_count = 0
        self.limit = len(self.data)

    def load_text(self):
        with open(self.filename) as file:
            data = file.read().splitlines()
            self.data = []
            polygon = []
            for index in range(0, len(data), 2):
                point = QPointF(
                    int(data[index]) * 10,
                    int(data[index + 1]) * 10
                )
                polygon.append(point)
                if len(polygon) >= 2 and polygon[0] == polygon[-1]:
                    self.data.append(QPolygonF(polygon))
                    polygon = []

    def load_json(self):
        import json
        pieces = None
        with open(self.filename, 'r') as file:
            pieces = json.load(file)

        answer = None
        with open(self.answerfile, 'r') as file:
            answer = json.load(file)

        for ope in answer['operations']:
            pos = QPointF(int(ope['pos']['x']), int(ope['pos']['y']))
            polygon = []
            piece = pieces['pieces'][int(ope['piece_id'])]
            piece = piece['shapes'][int(ope['shape_id'])]
            for p in piece:
                point = QPointF(int(p['x']), int(p['y'])) + pos
                polygon.append(point * 10)
            polygon.append(polygon[0])
            self.data.append(polygon)

    def paintEvent(self, event):
        super().paintEvent(event)
        painter = QPainter(self)
        opt = QStyleOption()
        opt.initFrom(self)
        self.style().drawPrimitive(QStyle.PE_Frame, opt, painter, self)
        if self.data is None:
            return

        painter.translate(10, 10)
        painter.setPen(Qt.white)
        painter.drawPolygon(self.data[0])

        painter.setBrush(Qt.black)
        for index in range(1, self.piece_count + 1):
            painter.drawPolygon(self.data[index])

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Right:
            self.piece_count += 1
            if self.piece_count >= self.limit:
                self.piece_count = self.limit - 1
        elif event.key() == Qt.Key_Left:
            self.piece_count -= 1
            if self.piece_count < 0:
                self.piece_count = 0
        self.update()

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    visualiser = Visualiser()
    visualiser.show()
    sys.exit(app.exec_())
