import sys
import logging
from PyQt5.QtWidgets import QApplication
from widgets import MainWindow

# logging.basicConfig(filename='output.log', level=logging.DEBUG)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
