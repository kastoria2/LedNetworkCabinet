#ifndef __BREATH_ANIMATION_H__
#define __BREATH_ANIMATION_H__

#include "Animation.h"

class BreathAnimation : public Animation
{
public:
  BreathAnimation();
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut);
};

#endif // #ifndef __BREATH_ANIMATION_H__