import time
from typing import List

class LedOut(object):
    '''
    Looks something like a Struct to hold parameters to control the 
    output of a LED. 
    '''

    def __init__(self, index:int, position:List[List[int]]):
        self.finalColor = 0

        self.index = index
        self.position = position

class InputParams(object):
    '''
    Looks something like a Struct to hold input parameters that can
    be used to compute animation state.
    '''
    
    def __init__(self):
        self.reset(0)

    def reset(self, currentTime_ms:int):

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

        # Some generic indicator of the speed for the anmitaion.
        # e.g. 'breath' could be faster or slower based on this
        # value.  Sin waves could adjust period based on this.
        self.speed = 128


GLOBAL_INPUT_PARAMS = InputParams()
GLOBAL_ANIMATION_METHOD = None
GLOBAL_LEDS = [LedOut(i, [0,0]) for i in range(98)]

def initAnimation(animationMethod, currentTime_ms:int = None):

    if currentTime_ms is None:
        currentTime_ms = int(round(time.time())*1000)

    global GLOBAL_INPUT_PARAMS
    GLOBAL_INPUT_PARAMS.reset(currentTime_ms)

    GLOBAL_INPUT_PARAMS.baseColor = 0xff0000

    global GLOBAL_ANIMATION_METHOD
    GLOBAL_ANIMATION_METHOD = animationMethod

def updateAnimation(currentTime_ms:int = None):
    
    if currentTime_ms is None:
        currentTime_ms = int(round(time.time())*1000)
    
    global GLOBAL_INPUT_PARAMS

    GLOBAL_INPUT_PARAMS.deltaTime_ms = GLOBAL_INPUT_PARAMS.currentTime_ms - currentTime_ms
    GLOBAL_INPUT_PARAMS.currentTime_ms = currentTime_ms

    global GLOBAL_ANIMATION_METHOD

    global GLOBAL_LEDS
    for led in GLOBAL_LEDS:
        GLOBAL_ANIMATION_METHOD(led, GLOBAL_INPUT_PARAMS)

def static(ledOut:LedOut, inputParams:InputParams):
    '''
    'Animation' that just displays a single, static color.
    '''
    ledOut.finalColor = inputParams.baaseColor

def breath(ledOut:LedOut, inputParams:InputParams):
    
    ledOut.finalColor = inputParams.baseColor
    
    CYCLE_PERIOD = 5000

    # Max period is 5s for a breath cycle.
    period = CYCLE_PERIOD * (inputParams.speed / 256.0)
    
    cycleTime = (inputParams.currentTime - inputParams.startTime) % period

    absPercentage = abs((period/2.0) - cycleTime) / period

    red = (inputParams.baseColor & 0xff0000) >> 16
    green = (inputParams.baseColor & 0x00ff00) >> 8
    blue = inputParams.baseColor & 0x0000ff

    red = int(red * absPercentage)
    green = int(green * absPercentage)
    blue = int(blue * absPercentage)

    ledOut.finalColor = red << 16 + green << 8 + blue
