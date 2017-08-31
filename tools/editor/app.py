import sys
import logging
from PyQt5.QtWidgets import QApplication
from widgets import MainWindow
from PyQt5.QtCore import QPointF
from PyQt5.QtGui import QPolygonF
from util import ConvexHull

logging.basicConfig(
    filename='output.log',
    filemode='w',
    level=logging.DEBUG,
    format='%(asctime)s:%(filename)s:%(lineno)d:%(message)s'
)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()

    convex = ConvexHull([
        '(0, 0)',
        '(2, 0)',
        '(3, 1)',
        '(3, 5)',
        '(0, 5)',
        '(5, 5)',
    ])

    print(convex.get())

    from util import convert_from_str
    print(convert_from_str('(10, 0)'))
    print(convert_from_str('(10, 12)'))
    print(convert_from_str('(10 12)'))

    sys.exit(app.exec())
