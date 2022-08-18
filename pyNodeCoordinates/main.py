# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from PySide2.QtGui import QGuiApplication
from PySide2.QtQml import QQmlApplicationEngine, qmlRegisterType, QQmlContext

from hexagons.hexcoords import Hexagon, HexPanel
from PyHexagon import PyHexagon

if __name__ == "__main__":
    qmlRegisterType(PyHexagon, "com.fathom.hexagonpanel", 1, 0, "PyHexagon")
    qmlRegisterType(Hexagon, "com.fathom.hexagonpanel", 1, 0, "HexagonModel")
    qmlRegisterType(HexPanel, "com.fathom.hexagonpanel", 1, 0, "HexagonPanelModel")

    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    engine.rootContext().setContextProperty("globalStringTest", "zens11")

    hexagonPanelModel = HexPanel(None, 1000, 1000, 50)
    engine.rootContext().setContextProperty("GlobalHexagonPanelModel", hexagonPanelModel)

    hexagonPanelModel = HexPanel
    engine.load(os.fspath(Path(__file__).resolve().parent / "main.qml"))


    if not engine.rootObjects():
        sys.exit(-1)
    sys.exit(app.exec_())
