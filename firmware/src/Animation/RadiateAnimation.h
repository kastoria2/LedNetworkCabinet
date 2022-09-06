#ifndef _RADIATE_ANIMATION_H_
#define _RADIATE_ANIMATION_H_

#include "Animation.h"

class RadiateAnimation : public Animation
{
public:
  RadiateAnimation();
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut);

private:
};

#endif  // #ifndef _RADIATE_ANIMATION_H_