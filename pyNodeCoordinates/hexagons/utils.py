import math
from typing import List


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