#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include <Arduino.h>
#include "LedLocations.h"

#include "InputParams.h"

#include "Animation.h"

class Animations
{
public:

  Animations(LedOut leds[], int ledCount);

  InputParams& getInputParams();

  void update();

private:

  InputParams inputParams;
  LedOut* leds;
  int ledCount;

  Animation* currentAnimation;
  Animation* animations[3];
};

extern InputParams inputParams;

void updateAnimation(InputParams& inputParams, LedOut leds[]);

// void animation_breath(const InputParams& inputParams, LedOut& ledOut);
// void animation_radiate(const InputParams& inputParams, LedOut& ledOut);

#endif  // __ANIMATIONS_H__
