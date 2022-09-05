#ifndef __STATIC_ANIMATION_H__
#define __STATIC_ANIMATION_H__

#include "Animation.h"

class StaticAnimation : public Animation
{
public:
  StaticAnimation();
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut);
};

#endif // #ifndef __STATIC_ANIMATION_H__