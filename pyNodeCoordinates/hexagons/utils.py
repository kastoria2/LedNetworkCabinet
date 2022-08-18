import math

def rotateVector2d(vec:list[float], radians) -> list[float]:
    return [
        vec[0] * math.cos(radians) - vec[1] * math.sin(radians),
        vec[0] * math.sin(radians) + vec[1] * math.cos(radians)
    ]

def hexagonHeight(radius_mm):
    return radius_mm * (1 / 1.1547005)
