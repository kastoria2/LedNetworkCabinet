#include "utils.h"

#include <Arduino.h>

float distance(const float a[], const float b[])
{
  float xDelta = b[0] - a[0];
  float yDelta = b[1] - a[1];

  return sqrt(xDelta * xDelta + yDelta * yDelta);
}

float clamp(float v, float min, float max)
{
  if (v < min)
  {
    return min;
  }

  if (v > max)
  {
    return max;
  }

  return v;
}

float blend(float a, float b, float t)
{
  return (a * t) + (b * (1.0 - t));
}

void argb2components(uint32_t argb, uint8_t* components)
{
  components[0] = (argb & 0xFF000000) >> 24;
  components[1] = (argb & 0x00FF0000) >> 16;
  components[2] = (argb & 0x0000FF00) >> 8;
  components[3] = (argb & 0x000000FF);
}

uint32_t components2argb(uint8_t* components)
{
  return (components[0] << 24) + (components[1] << 16) + (components[2] << 8) + components[3];
}

uint32_t blendComponents(uint32_t a, uint32_t b, float t)
{
  uint8_t c1[4];
  uint8_t c2[4];

  argb2components(a, c1);
  argb2components(b, c2);

  uint8_t cR[4] = {
    uint8_t(blend(c1[0], c2[0], t)),
    uint8_t(blend(c1[1], c2[1], t)),
    uint8_t(blend(c1[2], c2[2], t)),
    uint8_t(blend(c1[3], c2[3], t))
  };

  return components2argb(cR);
}