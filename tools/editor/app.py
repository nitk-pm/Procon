import os
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

if not os.path.isfile('editor_rc.py'):
    os.system('pyrcc5 -o editor_rc.py editor.qrc')

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
