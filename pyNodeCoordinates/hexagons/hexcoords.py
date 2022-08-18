import math

from .utils import hexagonHeight, rotateVector2d
from PySide2.QtCore import QObject

class Hexagon(QObject):
    '''
    A hexagon at a given spatial coordinate.
    '''

    def __init__(self, parent:QObject=None, x:float=0, y:float=0, radius:float=1):
        QObject.__init__(self, parent)

        self.x = x
        self.y = y
        self.radius = radius

    def vertices(self) -> list[list[float]]:
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

class HexPanel(object):
    '''
    Represents a physical hexagon panel.
    '''

    def __init__(self, width_mm:float, height_mm:float, radius_mm:float):
        self.width_mm = width_mm
        self.height_mm = height_mm
        self.radius_mm = radius_mm

    def diameter(self):
        return self.radius_mm * 2

    def columns(self):
        return self.width_mm / self.diameter()

    def rows(self):
        return self.height_mm / (hexagonHeight(self.radius_mm) * 2)

    def hexagonAtIndex(self, column, row):
        x = (column * self.radius_mm * 3) + (column+1)%2 * self.radius_mm * 1.5
        y = row * hexagonHeight(self.radius_mm) / 2
        return Hexagon(x, y, self.radius_mm)