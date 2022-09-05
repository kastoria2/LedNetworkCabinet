#include "Animations.h"

InputParams inputParams;

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

void updateAnimation(InputParams &inputParams, LedOut leds[])
{
  // Book keeping for animation parameters
  uint32_t currentMillis = millis();

  inputParams.deltaTime_ms = currentMillis - inputParams.currentTime_ms;
  inputParams.currentTime_ms = currentMillis;

  for (int i = 0; i < NUM_LEDS; i++)
  {
    animation_radiate(inputParams, leds[i]);
  }
}

float origin[] = {0, 0};
float lowerRight[] = {625, 736};
float maxDist = distance(origin, lowerRight);

void animation_breath(const InputParams &inputParams, LedOut &ledOut)
{
  uint32_t CYCLE_PERIOD = 6000;

  // Max period is 5s for a breath cycle.
  uint32_t period = int(CYCLE_PERIOD * ((256 - inputParams.speed) / 255.0));

  uint32_t cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period;

  float absPercentage = (abs((period / 2.0) - cycleTime) / period) * 2;

  uint8_t red = (inputParams.color & 0xff0000) >> 16;
  uint8_t green = (inputParams.color & 0x00ff00) >> 8;
  uint8_t blue = inputParams.color & 0x0000ff;

  red = int(red * absPercentage);
  green = int(green * absPercentage);
  blue = int(blue * absPercentage);

  ledOut.color = NeoPixels::Color(red, green, blue);
}

float blend(float a, float b, float t)
{
  return (a * t) + (b * (1.0 - t));
}

void animation_radiate(const InputParams &inputParams, LedOut &ledOut)
{
  uint32_t CYCLE_PERIOD = 5000;

  uint32_t period = int(CYCLE_PERIOD * ((256 - inputParams.speed) / 255.0));
  uint32_t cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period;
  float absPercentage = (float(cycleTime) / period);

  float ledDist = distance(inputParams.baseLocation_mm, ledOut.position_mm);
  float ledPercent = ledDist / maxDist;

  uint8_t red = (inputParams.color & 0xff0000) >> 16;
  uint8_t green = (inputParams.color & 0x00ff00) >> 8;
  uint8_t blue = inputParams.color & 0x0000ff;

  float scaleFactor = clamp((1 - abs(absPercentage - ledPercent) * 5), 0, 1.0);

  uint8_t bgRed = (inputParams.bgColor & 0xff0000) >> 16;
  uint8_t bgGreen = (inputParams.bgColor & 0x00ff00) >> 8;
  uint8_t bgBlue = (inputParams.bgColor & 0x0000ff);

  red = blend(red, bgRed, scaleFactor);
  green = blend(green, bgGreen, scaleFactor);
  blue = blend(blue, bgBlue, scaleFactor);

  ledOut.color = NeoPixels::Color(red, green, blue);
}