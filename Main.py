from gui import Window
from PyQt6.QtWidgets import QApplication
import sys



if(__name__ == "__main__"):
    app = QApplication(sys.argv)
    window = Window.Window()
    window.show()
    sys.exit(app.exec())



