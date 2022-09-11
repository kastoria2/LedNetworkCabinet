#include "StaticAnimation.h"
#include "HexPanel.h"

StaticAnimation::StaticAnimation(HexPanel& hexPanel) : Animation("Static", hexPanel)
{}

void StaticAnimation::updateLed(const InputParams& inputParams, LedOut& ledOut)
{
  ledOut.color = inputParams.color;
}
