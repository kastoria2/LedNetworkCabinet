import math

from .utils import hexagonHeight, rotateVector2d
from typing import List
from PySide2.QtCore import QObject, Property, Signal, Slot

class Hexagon(QObject):
    '''
    A hexagon at a given spatial coordinate.
    '''

    def __init__(self, parent:QObject=None, x:float=0, y:float=0, radius:float=1):
        QObject.__init__(self, parent)

        self.x = x
        self.y = y
        self.radius = radius

    @Slot(result=float)
    def getX(self):
        return self.x

    @Slot(result=float)
    def getY(self):
        return self.y

    @Slot(result=float)
    def getRadius(self):
        return self.radius

    @Slot(result=float)
    def getDiameter(self):
        return self.radius * 2

    @Slot(result=float)
    def getHeight(self):
        return self.radius * 2 * (1/1.1547005)

    def getVertices(self) -> List[List[float]]:
        '''
        Returns the vertices that comprise the hexagon.  Each vertex is a
        list of [x,y].
        You get a list of 6 of those vertices.
        '''
        p0 = [self.radius, 0.0]

        points = []
        for i in range(6):
            hexagonSpace = rotateVector2d(p0, i * math.pi/3)
            worldSpace = [hexagonSpace[0] + self.x, hexagonSpace[1]+self.y] 
            points.append(worldSpace)

        return points

    def getLedVertices(self) -> List[List[float]]:
        '''
        Returns the vertices that have LEDs 'owned' by this
        hexagon.
        '''
        verts = self.getVertices()
        return [verts[2], verts[3]]

    verticesChanged = Signal()
    vertices = Property("QVariantList", getVertices, verticesChanged)
    ledVertices = Property("QVariantList", getLedVertices, verticesChanged)

class HexPanel(QObject):
    '''
    Represents a physical hexagon panel.
    '''

    def __init__(self, 
        parent:QObject=None,
        width_mm:float=1000,
        height_mm:float=1000,
        radius_mm:float=50):

        QObject.__init__(self, parent)
        self.width_mm = width_mm
        self.height_mm = height_mm
        self.radius_mm = radius_mm

    @Slot(result=float)
    def width(self):
        return self.width_mm

    @Slot(result=float)
    def height(self):
        return self.height_mm

    @Slot(result=float)
    def radius(self):
        return self.radius_mm

    @Slot(result=float)
    def diameter(self):
        return self.radius_mm * 2

    @Slot(result=int)
    def columns(self):
        return self.width_mm / (self.diameter() + self.radius())

    @Slot(result=int)
    def rows(self):
        return self.height_mm / ((hexagonHeight(self.radius_mm) ))

    @Slot(int, int, result=Hexagon)
    def hexagonAtIndex(self, column, row):
        x = (column * self.radius_mm * 3) + ((row+1)%2) * self.radius_mm * 1.5
        y = row * hexagonHeight(self.radius_mm) / 2
        return Hexagon(self, x, y, self.radius_mm)
