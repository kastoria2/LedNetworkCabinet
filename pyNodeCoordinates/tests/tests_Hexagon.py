import unittest

from hexagons.hexcoords import Hexagon

class HexagonTests(unittest.TestCase):

    def test_hexagon_at_origin(self):
        hexagon = Hexagon(None, 0, 0, 10)

        vertices = hexagon.vertices()

        self.assertEqual(len(vertices), 6)

        self.assertAlmostEqual(vertices[0][0], 10); self.assertAlmostEqual(vertices[0][1], 0)
        self.assertAlmostEqual(vertices[1][0], 5); self.assertAlmostEqual(vertices[1][1], 8.660254037844386)
        self.assertAlmostEqual(vertices[2][0], -5); self.assertAlmostEqual(vertices[2][1], 8.660254037844386)
        self.assertAlmostEqual(vertices[3][0], -10); self.assertAlmostEqual(vertices[3][1], 0)
        self.assertAlmostEqual(vertices[4][0], -5); self.assertAlmostEqual(vertices[4][1], -8.660254037844386)
        self.assertAlmostEqual(vertices[5][0], 5); self.assertAlmostEqual(vertices[5][1], -8.660254037844386)

    def test_hexagon_quadrant_1(self):
        hexagon = Hexagon(None, 10, 10, 10)
        vertices = hexagon.vertices()

        self.assertEqual(len(vertices), 6)

        self.assertAlmostEqual(vertices[0][0], 20); self.assertAlmostEqual(vertices[0][1], 10)
        self.assertAlmostEqual(vertices[1][0], 15); self.assertAlmostEqual(vertices[1][1], 18.660254037844386)
        self.assertAlmostEqual(vertices[2][0], 5); self.assertAlmostEqual(vertices[2][1], 18.660254037844386)
        self.assertAlmostEqual(vertices[3][0], 0); self.assertAlmostEqual(vertices[3][1], 10)
        self.assertAlmostEqual(vertices[4][0], 5); self.assertAlmostEqual(vertices[4][1], 10-8.660254037844386)
        self.assertAlmostEqual(vertices[5][0], 15); self.assertAlmostEqual(vertices[5][1], 10-8.660254037844386)