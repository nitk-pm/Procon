# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(960, 720)
        MainWindow.setStyleSheet("")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtWidgets.QGridLayout(self.centralwidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.view = View(self.centralwidget)
        self.view.setStyleSheet("background-color: rgb(66, 66, 66);")
        self.view.setFrameShape(QtWidgets.QFrame.Panel)
        self.view.setFrameShadow(QtWidgets.QFrame.Raised)
        self.view.setLineWidth(1)
        self.view.setMidLineWidth(0)
        self.view.setTransformationAnchor(QtWidgets.QGraphicsView.NoAnchor)
        self.view.setObjectName("view")
        self.gridLayout.addWidget(self.view, 0, 0, 1, 1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.toolBar = QtWidgets.QToolBar(MainWindow)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.toolBar.sizePolicy().hasHeightForWidth())
        self.toolBar.setSizePolicy(sizePolicy)
        self.toolBar.setMinimumSize(QtCore.QSize(0, 0))
        self.toolBar.setStyleSheet("background-color: rgb(187, 222, 251);")
        self.toolBar.setMovable(False)
        self.toolBar.setAllowedAreas(QtCore.Qt.AllToolBarAreas)
        self.toolBar.setIconSize(QtCore.QSize(36, 48))
        self.toolBar.setToolButtonStyle(QtCore.Qt.ToolButtonFollowStyle)
        self.toolBar.setFloatable(True)
        self.toolBar.setObjectName("toolBar")
        MainWindow.addToolBar(QtCore.Qt.LeftToolBarArea, self.toolBar)
        self.action_new = QtWidgets.QAction(MainWindow)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/actions/icons/new.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_new.setIcon(icon)
        self.action_new.setObjectName("action_new")
        self.action_save = QtWidgets.QAction(MainWindow)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/actions/icons/save.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_save.setIcon(icon1)
        self.action_save.setObjectName("action_save")
        self.action_open = QtWidgets.QAction(MainWindow)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/actions/icons/open.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_open.setIcon(icon2)
        self.action_open.setObjectName("action_open")
        self.action_preview = QtWidgets.QAction(MainWindow)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(":/actions/icons/preview.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_preview.setIcon(icon3)
        self.action_preview.setObjectName("action_preview")
        self.action_undo = QtWidgets.QAction(MainWindow)
        self.action_undo.setEnabled(False)
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(":/actions/icons/undo.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_undo.setIcon(icon4)
        self.action_undo.setObjectName("action_undo")
        self.action_redo = QtWidgets.QAction(MainWindow)
        self.action_redo.setEnabled(False)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(":/actions/icons/redo.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_redo.setIcon(icon5)
        self.action_redo.setObjectName("action_redo")
        self.action_close = QtWidgets.QAction(MainWindow)
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(":/actions/icons/close.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_close.setIcon(icon6)
        self.action_close.setObjectName("action_close")
        self.action_delete = QtWidgets.QAction(MainWindow)
        self.action_delete.setEnabled(False)
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(":/actions/icons/delete.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_delete.setIcon(icon7)
        self.action_delete.setObjectName("action_delete")
        self.action_frame = QtWidgets.QAction(MainWindow)
        self.action_frame.setCheckable(True)
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(":/actions/icons/frame.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_frame.setIcon(icon8)
        self.action_frame.setObjectName("action_frame")
        self.toolBar.addAction(self.action_new)
        self.toolBar.addAction(self.action_open)
        self.toolBar.addAction(self.action_save)
        self.toolBar.addAction(self.action_close)
        self.toolBar.addAction(self.action_preview)
        self.toolBar.addAction(self.action_frame)
        self.toolBar.addAction(self.action_delete)
        self.toolBar.addAction(self.action_undo)
        self.toolBar.addAction(self.action_redo)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.toolBar.setWindowTitle(_translate("MainWindow", "toolBar"))
        self.action_new.setText(_translate("MainWindow", "new"))
        self.action_new.setToolTip(_translate("MainWindow", "new"))
        self.action_save.setText(_translate("MainWindow", "save"))
        self.action_save.setToolTip(_translate("MainWindow", "save"))
        self.action_open.setText(_translate("MainWindow", "open"))
        self.action_open.setToolTip(_translate("MainWindow", "open"))
        self.action_preview.setText(_translate("MainWindow", "preview"))
        self.action_preview.setToolTip(_translate("MainWindow", "preview"))
        self.action_undo.setText(_translate("MainWindow", "undo"))
        self.action_undo.setToolTip(_translate("MainWindow", "undo"))
        self.action_undo.setShortcut(_translate("MainWindow", "Ctrl+Z"))
        self.action_redo.setText(_translate("MainWindow", "redo"))
        self.action_redo.setToolTip(_translate("MainWindow", "redo"))
        self.action_redo.setShortcut(_translate("MainWindow", "Ctrl+Y"))
        self.action_close.setText(_translate("MainWindow", "close"))
        self.action_close.setToolTip(_translate("MainWindow", "close document"))
        self.action_delete.setText(_translate("MainWindow", "delete"))
        self.action_delete.setToolTip(_translate("MainWindow", "delete"))
        self.action_frame.setText(_translate("MainWindow", "edit frame"))
        self.action_frame.setToolTip(_translate("MainWindow", "edit frame"))

from widgets import View
import resource_rc