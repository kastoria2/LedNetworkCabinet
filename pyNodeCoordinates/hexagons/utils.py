import math
from typing import List, Tuple


def rotateVector2d(vec: List[float], radians) -> List[float]:
    return [
        vec[0] * math.cos(radians) - vec[1] * math.sin(radians),
        vec[0] * math.sin(radians) + vec[1] * math.cos(radians)
    ]


def hexagonHeight(radius_mm):
    return radius_mm * 2 * (1 / 1.1547005)


UM_IN_MM = 1000.0


def um2mm(um: int):
    return um / UM_IN_MM


def mm2um(mm: float):
    return int(mm * UM_IN_MM)


def blend(a: float, b: float, t: float):
    return (a * t) + (b * (1.0-t))


def argb2components(argb: int):
    return [
        (argb & 0xFF000000) >> 24,
        (argb & 0x00FF0000) >> 16,
        (argb & 0x0000FF00) >> 8,
        (argb & 0x000000FF)
    ]


def components2argb(a: int, r: int, g: int, b: int):
    return (a << 24) + (r << 16) + (g << 8) + b


def blendComponents(a: int, b: int, t: float):

    c1 = argb2components(a)
    c2 = argb2components(b)

    cR = [
        int(blend(c1[0], c2[0], t)),
        int(blend(c1[1], c2[1], t)),
        int(blend(c1[2], c2[2], t)),
        int(blend(c1[3], c2[3], t))
    ]

    return components2argb(*cR)


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

def multiplyComponents(factor:int, argb:int):
    a = (argb & 0xFF000000) >> 24
    r = (argb & 0x00FF0000) >> 16
    g = (argb & 0x0000FF00) >> 8
    b = (argb & 0x000000FF)



    a = int(clamp(a * factor, 0, 255)) << 24
    r = int(clamp(r * factor, 0, 255)) << 16
    g = int(clamp(g * factor, 0, 255)) << 8
    b = int(clamp(b * factor, 0, 255))

    return a + r + g + b