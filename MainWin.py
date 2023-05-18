import sys
import os
import platform
from PyQt5 import QtCore, QtGui, QtWidgets
from CoreUI import *

WINDOW_SIZE = 0 

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)

        # Set up the user interface from Qt Designer
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Remove the default title bar
        self.setWindowFlag(QtCore.Qt.FramelessWindowHint)
        #self.setAttribute(QtCore.Qt.WA_TranslucentBackground)

        # Bind the button events
        self.ui.MinimizeButton.clicked.connect(self.showMinimized)
        self.ui.MaximizeButton.clicked.connect(self.maximize_window)
        self.ui.CloseButton.clicked.connect(self.close)

        self.isMaximized = False

        def moveWindow(e):
            if(self.isMaximized == False):
                if(e.buttons() == QtCore.Qt.LeftButton):
                    self.move(self.pos() + e.globalPos() - self.clickPosition)
                    self.clickPosition = e.globalPos()
                    e.accept()


        self.ui.TitleBar.mouseMoveEvent = moveWindow

        self.show()

        

    def mousePressEvent(self,event):
        self.clickPosition = event.globalPos()

    def maximize_window(self):
        global WINDOW_SIZE
        win_status = WINDOW_SIZE

        if (win_status == 0):
            WINDOW_SIZE = 1
            self.showMaximized()
            self.isMaximized = True
            self.ui.MaximizeButton.setIcon(self.ui.minimizeicon)
        else:
            WINDOW_SIZE = 0
            self.showNormal()
            self.isMaximized = False
            self.ui.MaximizeButton.setIcon(self.ui.maximizeicon)
            
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    sys.exit(app.exec_())