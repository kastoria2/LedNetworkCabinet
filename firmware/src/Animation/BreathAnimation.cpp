#include "BreathAnimation.h"
#include "HexPanel.h"

BreathAnimation::BreathAnimation(HexPanel& hexPanel) : Animation("Breath", hexPanel)
{}

void BreathAnimation::updateLed(const InputParams& inputParams, LedOut& ledOut)
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