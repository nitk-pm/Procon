import sys
import logging
from PyQt5.QtWidgets import QApplication
from widgets import MainWindow
from PyQt5.QtCore import QLineF, QPointF

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

    # p0 = QPointF(11, 0)
    # p1 = QPointF(12, 5)
    # p2 = QPointF(9, 6)
    # p3 = QPointF(4, 3)
    # p4 = QPointF(15, 8)
    # p5 = QPointF(13, 2)

    # l1 = QLineF(p1, p0)
    # l2 = QLineF(p1, p2)
    # l3 = QLineF(p1, p3)
    # l4 = QLineF(p1, p4)
    # l5 = QLineF(p1, p5)

    # print('9, 6: {}'.format(l1.angleTo(l2)))
    # print('4, 3: {}'.format(l1.angleTo(l3)))
    # print('15, 8: {}'.format(l1.angleTo(l4)))
    # print('13, 2: {}'.format(l1.angleTo(l5)))

    sys.exit(app.exec_())
