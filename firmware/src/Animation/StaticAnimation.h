#ifndef __STATIC_ANIMATION_H__
#define __STATIC_ANIMATION_H__

#include "Animation.h"

class HexPanel;

class StaticAnimation : public Animation
{
public:
  StaticAnimation(HexPanel& hexPanel);
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut);
};

#endif // #ifndef __STATIC_ANIMATION_H__