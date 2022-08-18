import unittest

import math
from hexagons.utils import hexagonHeight, rotateVector2d

class utils_tests(unittest.TestCase):
    
    def test_hexagonHeight(self):
        
        radius = 100
        self.assertAlmostEqual(hexagonHeight(radius), 86.60254325688783)

    def test_rotateVector2d(self):
        vec = [1.0, 0.0]

        v1 = rotateVector2d(vec, 0.0)
        v2 = rotateVector2d(vec, math.pi / 4)
        v3 = rotateVector2d(vec, math.pi / 2)
        v4 = rotateVector2d(vec, math.pi)
        v5 = rotateVector2d(vec, 3/2 * math.pi)

        self.assertAlmostEqual(v1[0], 1.0); self.assertAlmostEqual(v1[1], 0.0)
        self.assertAlmostEqual(v2[0], 0.7071067811865476); self.assertAlmostEqual(v2[1], 0.7071067811865476)
        self.assertAlmostEqual(v3[0], 0.0); self.assertAlmostEqual(v3[1], 1.0)
        self.assertAlmostEqual(v4[0], -1.0); self.assertAlmostEqual(v4[1], 0.0)
        self.assertAlmostEqual(v5[0], 0.0); self.assertAlmostEqual(v5[1], -1.0)
