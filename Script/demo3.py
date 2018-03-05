from FishEngine import *
from FishEditor import *

from PyQt5 import QtGui, QtCore, QtWidgets
from PyQt5.QtWidgets import *
import sys

def Start():
    # importer = FBXImporter()
    # importer.Import('/Users/yushroom/program/Unity/MonumentVally/Assets/Model/models.fbx')
    # print(importer.GetMeshByName('halfCube'))
    # Create a Qt application

    app = QApplication([])
    listWidget = QListWidget()
    listWidget.show()
    ls = ['test', 'test2', 'test3']
    listWidget.addItems(ls)
    # listWidget.show()
    app.exec_()
    # sys.exit(app.exec_())