import sys
import logging
from PyQt5.QtWidgets import QApplication
from widgets import MainWindow
from PyQt5.QtCore import QLineF

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

    l1 = QLineF(0, 0, 10, 10)
    l2 = QLineF(10, 10, 20, 20)
    l3 = QLineF(10, 10, 10, 0)

    print(l1.angleTo(l2))
    print(l1.angleTo(l3))

    sys.exit(app.exec())
