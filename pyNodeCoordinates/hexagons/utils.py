import math
from typing import List


def rotateVector2d(vec: List[float], radians) -> List[float]:
    return [
        vec[0] * math.cos(radians) - vec[1] * math.sin(radians),
        vec[0] * math.sin(radians) + vec[1] * math.cos(radians)
    ]


def hexagonHeight(radius_mm):
    return radius_mm * 2 * (1 / 1.1547005)
