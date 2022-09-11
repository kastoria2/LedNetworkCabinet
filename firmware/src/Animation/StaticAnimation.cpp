#include "StaticAnimation.h"
#include "HexPanel.h"

StaticAnimation::StaticAnimation(HexPanel& hexPanel) : Animation("Static", hexPanel)
{}

void StaticAnimation::update(const InputParams& inputParams)
{
  for(int i = 0; i < this->hexPanel.getLedCount(); i++)
  {
    LedOut& ledOut = this->hexPanel.getLedStrip()[i];
    ledOut.color = inputParams.color;
  }
}