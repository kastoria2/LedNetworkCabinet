#include "RadiateAnimation.h"
#include "HexPanel.h"

#include "utils.h"

// HACK
float origin[] = {0, 0};
float lowerRight[] = {625, 736};
float maxDist = distance(origin, lowerRight);

RadiateAnimation::RadiateAnimation(HexPanel& hexPanel) : Animation("Radiate", hexPanel)
{}

void RadiateAnimation::update(const InputParams& inputParams)
{
  for(int i = 0; i < this->hexPanel.getLedCount(); i++)
  {
    LedOut& ledOut = this->hexPanel.getLedStrip()[i];

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
}