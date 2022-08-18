import math
import time
from typing import List

from PySide2.QtCore import QObject, Property, Signal


class LedOut(QObject):
    '''
    Looks something like a Struct to hold parameters to control the
    output of a LED.
    '''

    def __init__(self, parent: QObject=None, index: int=-1, position: List[List[float]]=[0, 0]):

        QObject.__init__(self, parent)

        self._finalColor = 0

        self._index = index
        self._position = position

    def setFinalColor(self, value):
        self._finalColor = value
        self.finalColorChanged.emit()

    def setIndex(self, value):
        self._index = value
        self.indexChanged.emit()

    finalColorChanged = Signal()
    finalColor = Property("float", fget=lambda self: self._finalColor, fset=setFinalColor, notify=finalColorChanged)

    indexChanged = Signal()
    index = Property("int", fget=lambda self: self._index, fset=setIndex, notify=indexChanged)

    positionChanged = Signal()
    position = Property("QVariantList", fget=lambda self: self._position, notify=positionChanged)


class InputParams(object):
    '''
    Looks something like a Struct to hold input parameters that can
    be used to compute animation state.
    '''

    def __init__(self):
        self.reset(0)

    def reset(self, currentTime_ms: int):

        # The start of the animation cycle.
        self.startTime_ms = currentTime_ms

        # Current time in ms
        self.currentTime_ms = currentTime_ms

        # Time since last call in ms.
        self.deltaTime_ms = 0

        # Basic color for the animation
        self.baseColor = 0xff0000

        # If there is an accent color used in the animation.
        self.accentColor = 0

        # Generic location.  Useful for generating waves or gradients
        self.baseLocation = [0, 0]

        # Some generic indicator of the speed for the anmitaion.
        # e.g. 'breath' could be faster or slower based on this
        # value.  Sin waves could adjust period based on this.
        self.speed = 128


GLOBAL_INPUT_PARAMS = InputParams()
GLOBAL_ANIMATION_METHOD = None

def initAnimation(animationMethod, currentTime_ms: int = None):

    if currentTime_ms is None:
        currentTime_ms = int(time.time()*1000)

    global GLOBAL_INPUT_PARAMS
    GLOBAL_INPUT_PARAMS.reset(currentTime_ms)

    GLOBAL_INPUT_PARAMS.baseColor = 0xff00ff
    GLOBAL_INPUT_PARAMS.baseLocation = [625/2.0, 732/2.0]

    global GLOBAL_ANIMATION_METHOD
    GLOBAL_ANIMATION_METHOD = animationMethod


def updateAnimation(ledStrip: List[LedOut], currentTime_ms: int = None):

    if currentTime_ms is None:
        currentTime_ms = int(time.time()*1000)

    # print(f"currentTime_ms: {currentTime_ms}")

    global GLOBAL_INPUT_PARAMS

    GLOBAL_INPUT_PARAMS.deltaTime_ms = currentTime_ms - GLOBAL_INPUT_PARAMS.currentTime_ms
    GLOBAL_INPUT_PARAMS.currentTime_ms = currentTime_ms

    global GLOBAL_ANIMATION_METHOD

    for led in ledStrip:
        GLOBAL_ANIMATION_METHOD(led, GLOBAL_INPUT_PARAMS)


def static(ledOut: LedOut, inputParams: InputParams):
    '''
    'Animation' that just displays a single, static color.
    '''

    ledOut.finalColor = inputParams.baseColor


def breath(ledOut: LedOut, inputParams: InputParams):

    CYCLE_PERIOD = 10000

    # Max period is 5s for a breath cycle.
    period = int(CYCLE_PERIOD * (inputParams.speed / 256.0))

    cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period

    absPercentage = (abs((period/2.0) - cycleTime) / period) * 2

    red = (inputParams.baseColor & 0xff0000) >> 16
    green = (inputParams.baseColor & 0x00ff00) >> 8
    blue = inputParams.baseColor & 0x0000ff

    # print(f"period: {period}, cycleTime: {cycleTime}, absPercentage: {absPercentage}, red: {red}, green: {green}, blue: {blue}")

    red = int(red * absPercentage)
    green = int(green * absPercentage)
    blue = int(blue * absPercentage)

    ledOut.finalColor = (red << 16) + (green << 8) + blue


def indexBreath(ledOut: LedOut, inputParams: InputParams):
    CYCLE_PERIOD = 20000

    period = int(CYCLE_PERIOD * (inputParams.speed / 256.0))
    cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period
    absPercentage = (cycleTime / period)

    if ledOut.index / 98.0 <= absPercentage:
        ledOut.finalColor = inputParams.baseColor
    else:
        ledOut.finalColor = 0


def distance(v1, v2):
    delta = [
        v2[0] - v1[0],
        v2[1] - v1[1]
    ]

    return math.sqrt(delta[0]**2 + delta[1]**2)


def clamp(value, lower, upper):
    if value < lower:
        return lower

    if value > upper:
        return upper

    return value

def radiate(ledOut: LedOut, inputParams: InputParams):
    CYCLE_PERIOD = 5000

    period = int(CYCLE_PERIOD * (inputParams.speed / 256.0))
    cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period
    absPercentage = (cycleTime / period)

    maxDist = distance([0, 0], [625, 736])
    ledDist = distance(inputParams.baseLocation, ledOut.position)
    ledPercent = ledDist / maxDist

    red = (inputParams.baseColor & 0xff0000) >> 16
    green = (inputParams.baseColor & 0x00ff00) >> 8
    blue = inputParams.baseColor & 0x0000ff

    scaleFactor = clamp((1 - math.fabs(absPercentage - ledPercent) * 5), 0, 1.0)

    red = int(red * scaleFactor)
    green = int(green * scaleFactor)
    blue = int(blue * scaleFactor)

    ledOut.finalColor = (red << 16) + (green << 8) + blue
