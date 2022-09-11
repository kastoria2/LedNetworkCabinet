#ifndef _RADIATE_ANIMATION_H_
#define _RADIATE_ANIMATION_H_

#include "Animation.h"

class HexPanel;

class RadiateAnimation : public Animation
{
public:
  RadiateAnimation(HexPanel& hexPanel);
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut);

private:
};

#endif  // #ifndef _RADIATE_ANIMATION_H_