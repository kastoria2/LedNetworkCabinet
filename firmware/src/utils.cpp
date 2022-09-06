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