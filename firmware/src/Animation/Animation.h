#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <Arduino.h>

#include "LedLocations.h"
#include "InputParams.h"

class Animation
{
public:
  Animation(const String& _displayName);
  virtual void updateLed(const InputParams& inputParams, LedOut& ledOut) = 0;

  const String& getDisplayName();

private:
  String displayName;
};

#endif  // #ifndef __ANIMATION_H__