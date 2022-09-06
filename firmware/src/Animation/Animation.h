#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "LedLocations.h"
#include "InputParams.h"

class Animation
{
public:
  Animation();
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut) = 0;
};

#endif  // #ifndef __ANIMATION_H__