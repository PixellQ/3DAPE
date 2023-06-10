import sys
import os
import platform
from PyQt5 import QtCore, QtGui, QtWidgets
from CoreUI import *
import QtModelView

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

        # Creating custom widgets
        self.ModelViewPort = QtModelView.QtModelViewPort(self.ui.ModelViewer)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ModelViewPort.sizePolicy().hasHeightForWidth())
        self.ModelViewPort.setSizePolicy(sizePolicy)
        self.ModelViewPort.setObjectName("ModelViewPort")
        self.ui.gridLayout_15.addWidget(self.ModelViewPort, 0, 0, 1, 1)

        # Bind the button events
        self.ui.MinimizeButton.clicked.connect(self.showMinimized)
        self.ui.MaximizeButton.clicked.connect(self.maximize_window)
        self.ui.CloseButton.clicked.connect(self.close)

        self.ui.ImportModelBtn.clicked.connect(self.open_model_dir)

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


    def open_model_dir(self):
        filename, ok = QtWidgets.QFileDialog.getOpenFileName(
            self.ModelViewPort,
            "Select a 3D File",
            "",
            "Videos (*.fbx *.obj)"
        )
        if filename:
            self.ModelViewPort.changeFile(filename)
            print(filename)
            

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    sys.exit(app.exec_())
