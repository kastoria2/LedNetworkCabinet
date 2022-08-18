# This Python file uses the following encoding: utf-8

from PySide2.QtQuick import QQuickPaintedItem
from PySide2.QtGui import QPainterPath, QColor


class PyHexagon(QQuickPaintedItem):
    def __init__(self, parent=None):
        super(PyHexagon, self).__init__(parent)

    def paint(self, painter):
        path = QPainterPath()
        path.moveTo(0, 0)
        path.lineTo(100, 0)
        path.lineTo(100, 100)
        path.lineTo(0, 100)
        painter.fillPath(path, QColor("red"))
