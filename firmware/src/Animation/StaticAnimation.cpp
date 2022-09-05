#include "StaticAnimation.h"

StaticAnimation::StaticAnimation()
{}

void StaticAnimation::updateLed(const InputParams& inputParams, LedOut& ledOut)
{
  ledOut.color = inputParams.color;
}
