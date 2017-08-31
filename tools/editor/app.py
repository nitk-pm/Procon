import sys
import logging
from PyQt5.QtWidgets import QApplication
from widgets import MainWindow

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

    s1 = ['a', 'b', 'a']
    print(s1)
    s2 = set(s1)
    print(s2)
    print(s1)
    sys.exit(app.exec())
