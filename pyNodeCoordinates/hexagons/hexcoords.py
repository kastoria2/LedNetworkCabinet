import math

from .utils import hexagonHeight, rotateVector2d
from typing import List
from PySide2.QtCore import QObject, Property, Signal, Slot

from .animations import initAnimation, updateAnimation, LedOut
from .animations import StaticAnimation, BreathAnimation, RadiateAnimation, BasePointAnimation, TwinkleAnimation


class Hexagon(QObject):
    '''
    A hexagon at a given spatial coordinate.
    '''

    def __init__(
            self,
            parent: QObject = None,
            x: float = 0,
            y: float = 0,
            radius: float = 1):
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
        return [verts[3], verts[4]]

    verticesChanged = Signal()
    vertices = Property("QVariantList", getVertices, notify=verticesChanged)
    ledVertices = Property("QVariantList", getLedVertices, notify=verticesChanged)


class HexPanel(QObject):
    '''
    Represents a physical hexagon panel.  Coordinate space is thought
    of as [0,0] at top left, [width,hegiht] is bottom right.  Similar
    to raster space. The hexagons are centered on the points.  So hex
    at index [0][0] could be at position [0,0].  This means that the
    vectices around that could be negative because the upper left vect
    will be "above and left" of the center (0,0) of the hexagon.
    '''

    def __init__(
            self,
            parent: QObject = None,
            width_mm: float = 1000,
            height_mm: float = 1000,
            radius_mm: float = 50):

        QObject.__init__(self, parent)
        self.width_mm = width_mm
        self.height_mm = height_mm
        self.radius_mm = radius_mm

        # Stored in row-major order.
        self.hexagons = []
        for row in range(self.rows()):
            currentRow = []
            for col in range(self.columns()):
                currentRow.append(self._hexagonAtIndex(col, row))
            self.hexagons.append(currentRow)

        self.ledStrip = self.generateLedStrip()

        initAnimation(TwinkleAnimation())

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
        return int(self.width_mm / (self.diameter() + self.radius()))

    @Slot(result=int)
    def rows(self):
        return int(self.height_mm / ((hexagonHeight(self.radius_mm)))) * 2

    @Slot(int, int, result=Hexagon)
    def _hexagonAtIndex(self, column, row):
        x = (column * self.radius_mm * 3) + ((row) % 2) * self.radius_mm * 1.5
        y = row * hexagonHeight(self.radius_mm) / 2

        x = x + self.radius_mm
        y = y + hexagonHeight(self.radius_mm) / 2

        return Hexagon(self, x, y, self.radius_mm)

    @Slot(int, int, result=Hexagon)
    def hexagonAtIndex(self, column, row):
        # print(f'len(self.hexagons): {len(self.hexagons)}, len(hex[0]): {len(self.hexagons[0])} column: {column}, row: {row}')
        return self.hexagons[row][column]

    @Slot()
    def update(self):
        updateAnimation(self.ledStrip)
        self.ledsChanged.emit()

    def getBounds(self):
        topLefts = [
            self.hexagonAtIndex(0, 0),
            self.hexagonAtIndex(0, 1)
        ]

        # print(f"topLefts: {topLefts}")

        bottomRights = [
            self.hexagonAtIndex(self.columns()-1, self.rows()-1),
            self.hexagonAtIndex(self.columns()-1, self.rows()-2)
        ]

        # print(f"bottomRights: {bottomRights}")

        # Hack force the bounds a tad smaller to beat comparing
        # 0.0 < 1.03*10^-5.
        epsilon = 0.001

        return [
            [
                min(topLefts[0].getX(), topLefts[1].getX()) + epsilon,
                min(topLefts[0].getY(), topLefts[1].getY()) + epsilon
            ],
            [
                max(bottomRights[0].getX(), bottomRights[1].getX()) - epsilon,
                max(bottomRights[0].getY(), bottomRights[1].getY()) - epsilon
            ]
        ]

    @Slot(result="QVariantList")
    def getLedVertices(self) -> List[Hexagon]:

        result = []

        bounds = self.getBounds()
        # print(bounds)

        def inBoundsExclusive(pt, topLeft, lowerRight):
            # print(f"{topLeft[1]} < {pt[1]} < {lowerRight[1]}")
            return topLeft[0] < pt[0] < lowerRight[0] \
                    and topLeft[1] < pt[1] < lowerRight[1]

        for row in range(0, self.rows()):
            for col in range(0, self.columns()):
                ledLocations = self.hexagonAtIndex(col, row).getLedVertices()
                result.extend(
                    filter(
                        lambda x:
                            inBoundsExclusive(x, bounds[0], bounds[1]),
                        ledLocations))

        return result

    def generateLedStrip(self) -> List[LedOut]:
        '''
        Generates a list of LedOut objects that are 0-index matched
        to LEDs on a physical strip that 'snakes' across rows
        in the logical model.
        '''

        ledVertices_mm = [[v[0], v[1]]
                          for v in self.getLedVertices()]

        # Sort the LEDs by Y coordinate into rows
        ledVertices_mm.sort(key=lambda l: l[1])

        minY = min([vert[1] for vert in ledVertices_mm])

        rows = []
        for vert in ledVertices_mm:
            rowIndex = round((vert[1] - minY) / (hexagonHeight(self.radius_mm) / 2))

            # Ensure there are indices to store into.
            while len(rows) <= rowIndex:
                rows.append([])

            rows[rowIndex].append(LedOut(None, -1, vert[:2]))

        # Sort the rows by X value alternating decreasing and increasing.
        for i in range(len(rows)):
            rows[i].sort(key=lambda lo: lo._position_mm[0], reverse=((i+1) % 2) == 0)

        # Flatten the results.
        result = []
        for row in rows:
            result.extend(row)

        # Assign indicies to the LED.
        for i in range(len(result)):
            result[i]._index = i

        return result

    def getLedStrip(self) -> List[LedOut]:
        # print(len(self.ledStrip))
        return self.ledStrip

    @Slot(result=str)
    def generateFirmwareDefinitions(self) -> str:

        buffer = [f"#define NUM_LEDS {len(self.ledStrip)}"]
        buffer.append("LedOut leds[NUM_LEDS] = {")

        for i in range(len(self.ledStrip)):
            led = self.ledStrip[i]

            buffer.append(f"LedOut {{{i}, {{{led.position_mm[0]},{led.position_mm[1]}}}, NeoPixels::Color(0,0,0)}},")

        buffer.append("};")

        buffer.append(f"width: {self.width_mm}, height: {self.height_mm}")

        return "\n".join(buffer)

    ledVerticesChanged = Signal()
    ledVertices = Property("QVariantList", getLedVertices, notify=ledVerticesChanged)

    ledsChanged = Signal()
    leds = Property("QVariantList", getLedStrip, notify=ledsChanged)
