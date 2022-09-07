#include "StaticAnimation.h"

StaticAnimation::StaticAnimation() : Animation("Static")
{}

void StaticAnimation::updateLed(const InputParams& inputParams, LedOut& ledOut)
{
  ledOut.color = inputParams.color;
}
