import math
import time
from typing import List

from PySide2.QtCore import QObject, Property, Signal

from .utils import blend

class LedOut(QObject):
    '''
    Looks something like a C 'struct' to hold parameters to control the
    output of a LED.
    '''

    def __init__(
            self, parent: QObject = None,
            index: int = -1,
            position_mm: List[List[float]] = [0, 0]):

        QObject.__init__(self, parent)

        self._color = 0

        self._index = index
        self._position_mm = position_mm

    def setFinalColor(self, value):
        self._color = value
        self.colorChanged.emit()

    def setIndex(self, value):
        self._index = value
        self.indexChanged.emit()

    colorChanged = Signal()
    color = Property(
        "float",
        fget=lambda self: self._color,
        fset=setFinalColor,
        notify=colorChanged)

    indexChanged = Signal()
    index = Property(
        "int",
        fget=lambda self: self._index,
        fset=setIndex,
        notify=indexChanged)

    positionChanged = Signal()
    position_mm = Property(
        "QVariantList",
        fget=lambda self: self._position_mm,
        notify=positionChanged)


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
        self.color = 0x000000

        # If there is an accent color used in the animation.
        self.bgColor = 0x000000

        # Generic location.  Useful for generating waves or gradients
        self.baseLocation_mm = [0, 0]

        # Some generic indicator of the speed for the anmitaion.
        # e.g. 'breath' could be faster or slower based on this
        # value.  Sin waves could adjust period based on this.
        self.speed = 128

class Animation():

    def __init__(self):
        pass

    def updateLed(self, inputParams: InputParams, ledOut: LedOut):
        ledOut.color = inputParams.color


GLOBAL_INPUT_PARAMS = InputParams()
GLOBAL_ANIMATION = None


def initAnimation(animation: Animation, currentTime_ms: int = None):

    if currentTime_ms is None:
        currentTime_ms = int(time.time()*1000)

    global GLOBAL_INPUT_PARAMS
    GLOBAL_INPUT_PARAMS.reset(currentTime_ms)

    GLOBAL_INPUT_PARAMS.color = 0xff00ff
    GLOBAL_INPUT_PARAMS.bgColor = 0x00ff00
    GLOBAL_INPUT_PARAMS.baseLocation_mm = [625 / 2, 736 / 2]

    global GLOBAL_ANIMATION
    GLOBAL_ANIMATION = animation


def updateAnimation(ledStrip: List[LedOut], currentTime_ms: int = None):

    if currentTime_ms is None:
        currentTime_ms = int(time.time()*1000)

    # print(f"currentTime_ms: {currentTime_ms}")

    global GLOBAL_INPUT_PARAMS

    GLOBAL_INPUT_PARAMS.deltaTime_ms = currentTime_ms - GLOBAL_INPUT_PARAMS.currentTime_ms
    GLOBAL_INPUT_PARAMS.currentTime_ms = currentTime_ms

    global GLOBAL_ANIMATION

    for led in ledStrip:
        GLOBAL_ANIMATION.updateLed(GLOBAL_INPUT_PARAMS, led)


class StaticAnimation(Animation):

    def updateLed(self, inputParams: InputParams, ledOut: LedOut):
        '''
        'Animation' that just displays a single, static color.
        '''

        ledOut.color = inputParams.color

class BreathAnimation(Animation):

    def updateLed(self, inputParams: InputParams, ledOut: LedOut):

        CYCLE_PERIOD = 10000

        # Max period is 5s for a breath cycle.
        period = int(CYCLE_PERIOD * (inputParams.speed / 256.0))

        cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period

        absPercentage = (abs((period/2.0) - cycleTime) / period) * 2

        red = (inputParams.color & 0xff0000) >> 16
        green = (inputParams.color & 0x00ff00) >> 8
        blue = inputParams.color & 0x0000ff

        # print(f"period: {period}, cycleTime: {cycleTime}, absPercentage: {absPercentage}, red: {red}, green: {green}, blue: {blue}")

        red = int(red * absPercentage)
        green = int(green * absPercentage)
        blue = int(blue * absPercentage)

        ledOut.color = (red << 16) + (green << 8) + blue

class IndexBreath(Animation):

    def updateLed(self, inputParams: InputParams, ledOut: LedOut):
        CYCLE_PERIOD = 20000

        period = int(CYCLE_PERIOD * (inputParams.speed / 256.0))
        cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period
        absPercentage = (cycleTime / period)

        if (ledOut.index / 98.0) <= absPercentage:
            ledOut.color = inputParams.color
        else:
            ledOut.color = 0


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

class RadiateAnimation(Animation):

    def updateLed(self, inputParams: InputParams, ledOut: LedOut):
        CYCLE_PERIOD = 5000

        period = int(CYCLE_PERIOD * (inputParams.speed / 256.0))
        cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period
        absPercentage = (cycleTime / period)

        maxDist = distance([0, 0], [625, 736])
        ledDist = distance(inputParams.baseLocation_mm, ledOut.position_mm)
        ledPercent = ledDist / maxDist

        red = (inputParams.color & 0xff0000) >> 16
        green = (inputParams.color & 0x00ff00) >> 8
        blue = inputParams.color & 0x0000ff

        scaleFactor = clamp((1 - math.fabs(absPercentage - ledPercent) * 10), 0, 1.0)

        bgRed = (inputParams.bgColor & 0xff0000) >> 16
        bgGreen = (inputParams.bgColor & 0x00ff00) >> 8
        bgBlue = (inputParams.bgColor & 0x0000ff)

        red = int(blend(red, bgRed, scaleFactor))
        green = int(blend(green, bgGreen, scaleFactor))
        blue = int(blend(blue, bgBlue, scaleFactor))

        ledOut.color = (red << 16) + (green << 8) + blue

class BasePointAnimation(Animation):

    def updateLed(self, inputParams: InputParams, ledOut: LedOut):

        red = (inputParams.color & 0xff0000) >> 16
        green = (inputParams.color & 0x00ff00) >> 8
        blue = inputParams.color & 0x0000ff

        ledDist = distance(inputParams.baseLocation_mm, ledOut.position_mm)
        maxDist = 60

        if ledOut.index == 45:
            pass

        ledOut.color = 0
        if ledDist < maxDist:
            ledOut.color = inputParams.color