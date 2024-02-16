# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Import.ui'
#
# Created by: PyQt5 UI code generator 5.15.9
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class QtImportDialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")

        Dialog.setWindowFlag(QtCore.Qt.FramelessWindowHint)
        Dialog.setAttribute(QtCore.Qt.WA_TranslucentBackground)

        Dialog.resize(600, 150)
        Dialog.setStyleSheet("#MainWidget{\n"
"   background-color: #ffffff;\n"
"   border:1px solid black;\n"
"   border-radius: 5px;\n"
"}\n"
"\n"
"#TitleWidget{\n"
"   background-color: #0078d7;\n"
"   border-top-left-radius: 5px;\n"
"   border-top-right-radius: 5px;\n"
"}\n"
"#Title{\n"
"   color: #fbfdfb;\n"
"}\n"
"\n"
"#TypeLabel{\n"
"   color: #000000;\n"
"}\n"
"\n"
"#FileLabel{\n"
"   color: #000000;\n"
"}\n")
        self.gridLayout = QtWidgets.QGridLayout(Dialog)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.MainWidget = QtWidgets.QWidget(Dialog)
        self.MainWidget.setObjectName("MainWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.MainWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 9)
        self.verticalLayout.setObjectName("verticalLayout")
        self.TitleWidget = QtWidgets.QWidget(self.MainWidget)
        self.TitleWidget.setObjectName("TitleWidget")
        self.gridLayout_3 = QtWidgets.QGridLayout(self.TitleWidget)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.Title = QtWidgets.QLabel(self.TitleWidget)
        font = QtGui.QFont()
        font.setFamily("Segoe UI")
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.Title.setFont(font)
        self.Title.setObjectName("Title")
        self.gridLayout_3.addWidget(self.Title, 0, 0, 1, 1)
        self.verticalLayout.addWidget(self.TitleWidget, 0, QtCore.Qt.AlignTop)
        self.ContentWidget = QtWidgets.QWidget(self.MainWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ContentWidget.sizePolicy().hasHeightForWidth())
        self.ContentWidget.setSizePolicy(sizePolicy)
        self.ContentWidget.setObjectName("ContentWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.ContentWidget)
        #self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.TypeWidget = QtWidgets.QWidget(self.ContentWidget)
        self.TypeWidget.setObjectName("TypeWidget")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.TypeWidget)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.TypeLabel = QtWidgets.QLabel(self.TypeWidget)
        self.TypeLabel.setObjectName("TypeLabel")
        self.horizontalLayout_2.addWidget(self.TypeLabel)
        self.comboBox = QtWidgets.QComboBox(self.TypeWidget)
        self.comboBox.addItem("FullBody") 
        self.comboBox.addItem("Face") 
        self.comboBox.addItem("Fingers")
        self.comboBox.setObjectName("comboBox")
        self.horizontalLayout_2.addWidget(self.comboBox)
        self.horizontalLayout.addWidget(self.TypeWidget)
        self.FileWidget = QtWidgets.QWidget(self.ContentWidget)
        self.FileWidget.setObjectName("FileWidget")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout(self.FileWidget)
        self.horizontalLayout_3.setContentsMargins(-1, -1, 0, -1)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.FileLabel = QtWidgets.QLabel(self.FileWidget)
        self.FileLabel.setObjectName("FileLabel")
        self.horizontalLayout_3.addWidget(self.FileLabel)
        self.lineEdit = QtWidgets.QLineEdit(self.FileWidget)
        self.lineEdit.setObjectName("lineEdit")
        self.horizontalLayout_3.addWidget(self.lineEdit)
        self.BrowseBtn = QtWidgets.QPushButton(self.FileWidget)
        self.BrowseBtn.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("UI/Icons/folderBu.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.BrowseBtn.setIcon(icon)
        self.BrowseBtn.setIconSize(QtCore.QSize(18, 18))
        self.BrowseBtn.setFlat(True)
        self.BrowseBtn.setObjectName("BrowseBtn")
        self.horizontalLayout_3.addWidget(self.BrowseBtn)
        self.horizontalLayout.addWidget(self.FileWidget)
        self.verticalLayout.addWidget(self.ContentWidget)
        self.ConfirmWidget = QtWidgets.QWidget(self.MainWidget)
        self.ConfirmWidget.setObjectName("ConfirmWidget")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.ConfirmWidget)
        self.gridLayout_2.setContentsMargins(-1, 0, -1, 3)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.buttonBox = QtWidgets.QDialogButtonBox(self.ConfirmWidget)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayout_2.addWidget(self.buttonBox, 0, 0, 1, 1)
        self.verticalLayout.addWidget(self.ConfirmWidget, 0, QtCore.Qt.AlignBottom)
        self.gridLayout.addWidget(self.MainWidget, 0, 0, 1, 1)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        self.Title.setText(_translate("Dialog", "Import Video File"))
        self.TypeLabel.setText(_translate("Dialog", "Type :"))
        self.FileLabel.setText(_translate("Dialog", "File : "))
        self.lineEdit.setText(_translate("Dialog", "Path"))



class QtVideoDetailWidget(object):
    def setupUi(self, VideoDetailWidget):
        self.VideoDetailWidget = VideoDetailWidget
        self.VideoDetailWidget.setObjectName("VideoDetail")
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.VideoDetailWidget.sizePolicy().hasHeightForWidth())
        self.VideoDetailWidget.setSizePolicy(sizePolicy)
        self.VideoDetailWidget.setStyleSheet("#VideoDetailWidget{\n"
"    background-color: #ffffff;\n"
"    border: 1px solid gray;\n"
"    border-radius: 5px;}\n"
"#VideoDetailWidget:hover{\n"
"    background-color: lightgray;\n"
"    border: 1px solid gray;\n"
"    border-radius: 5px;}")
        self.VideoDetailWidget.setObjectName("VideoDetailWidget")
        self.gridLayout_28 = QtWidgets.QGridLayout(self.VideoDetailWidget)
        self.gridLayout_28.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_28.setSpacing(0)
        self.gridLayout_28.setObjectName("gridLayout_28")
        self.VideoDetailButton = QtWidgets.QPushButton(self.VideoDetailWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.VideoDetailButton.sizePolicy().hasHeightForWidth())
        self.VideoDetailButton.setSizePolicy(sizePolicy)
        self.VideoDetailButton.setMinimumSize(QtCore.QSize(0, 100))
        self.VideoDetailButton.setStyleSheet("#VideoDetailButton\n"
"{\n"
"    background-position: center;\n"
"    background-repeat: no-repeat;\n"
"    border: none;\n"
"    border-radius: 5px;\n"
"    text-align: center;\n"
"    padding: -20px;\n"
"}")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("Tests/fc10e406-8445-4868-a2d4-526497fcd9bc_Untitled.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.VideoDetailButton.setIcon(icon)
        self.VideoDetailButton.setIconSize(QtCore.QSize(120, 120))
        self.VideoDetailButton.setObjectName("VideoDetailButton")
        self.gridLayout_28.addWidget(self.VideoDetailButton, 0, 0, 1, 1)
        self.VideoDetailBar = QtWidgets.QProgressBar(self.VideoDetailWidget)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.VideoDetailBar.sizePolicy().hasHeightForWidth())
        self.VideoDetailBar.setSizePolicy(sizePolicy)
        self.VideoDetailBar.setStyleSheet("#VideoDetailBar{\n"
"    margin: 10px;\n"
"    margin-top: 0px;\n"
"    margin-bottom: 10px;\n"
"    padding-bottom: -10px;\n"
"    border: 1px solid gray;\n"
"    border-radius: 2px;}\n"
"\n"
"#VideoDetailBar::chunk{\n"
"    background-color: rgb(0, 120, 215);\n"
"    margin-top: 0px;\n"
"    margin-bottom: 10px;\n"
"    padding-bottom: -10px;\n"
"    border: 1px solid gray;\n"
"    border-color: rgb(0, 120, 215);\n"
"    border-radius: 1px;}")
        self.VideoDetailBar.setProperty("value", 0)
        self.VideoDetailBar.setAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignTop)
        self.VideoDetailBar.setObjectName("VideoDetailBar")
        self.gridLayout_28.addWidget(self.VideoDetailBar, 1, 0, 1, 1)
        self.VideoDetailBar.raise_()
        self.VideoDetailButton.raise_()

        self.retranslateUi(self.VideoDetailWidget)
        QtCore.QMetaObject.connectSlotsByName(self.VideoDetailWidget)

    def onClickDetails(self, clicked: bool):
        if clicked:
            self.VideoDetailWidget.setStyleSheet("#VideoDetailWidget{\n"
                                                "    background-color: #ffffff;\n"
                                                "    border: 1px solid gray;\n"
                                                "    border-radius: 5px;\n"
                                                "    border-color: rgb(0, 120, 215);}")
        else:
            self.VideoDetailWidget.setStyleSheet("#VideoDetailWidget{\n"
                                                "    background-color: #ffffff;\n"
                                                "    border: 1px solid gray;\n"
                                                "    border-radius: 5px;}\n"
                                                "#VideoDetailWidget:hover{\n"
                                                "    background-color: lightgray;\n"
                                                "    border: 1px solid gray;\n"
                                                "    border-radius: 5px;}")

    def retranslateUi(self, VideoDetailWidget):
        _translate = QtCore.QCoreApplication.translate
        VideoDetailWidget.setWindowTitle(_translate("VideoDetail", "VideoDetail"))
        self.VideoDetailButton.setText(_translate("VideoDetail", "Finger : Armbamd.mp4\n"
"Type   : .mp4\n"
"Frames: 123 frames\n"
"codec  : mvaro"))



class QtPageButton(QtWidgets.QPushButton):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.gridLayout_14 = QtWidgets.QGridLayout(parent)
        self.gridLayout_14.setContentsMargins(0, 10, 0, 10)
        self.gridLayout_14.setSpacing(0)
        self.gridLayout_14.setObjectName("gridLayout_14")
        self.PageBtn = QtWidgets.QPushButton(parent)
        self.PageBtn.setStyleSheet("")
        self.PageBtn.setIconSize(QtCore.QSize(22, 22))
        self.PageBtn.setObjectName("PageBtn")
        self.gridLayout_14.addWidget(self.PageBtn, 0, 0, 1, 1)

    def setIcon(self, IconAddress):
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(IconAddress), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.PageBtn.setIcon(icon1)

    def startAnimation(self, current_size, target_size):
        animation = QtCore.QPropertyAnimation(self.btn, b'iconSize')
        animation.setStartValue(current_size)
        animation.setEndValue(target_size)
        animation.setDuration(500)
        animation.setEasingCurve(QtCore.QEasingCurve.InOutCubic)
        animation.start()

    def enterEvent(self, event):
        self.startAnimation(QtCore.QSize(22, 22), QtCore.QSize(28, 28))

    def leaveEvent(self, event):
        self.startAnimation(QtCore.QSize(28, 28), QtCore.QSize(22, 22))