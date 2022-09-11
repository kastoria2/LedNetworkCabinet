#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include "LedLocations.h"

#include "InputParams.h"

#include "Animation.h"

#define ANIMATION_COUNT 3

class HexPanel;

class Animations
{
public:

  Animations(HexPanel& hexPanel, int ledCount);

  InputParams& getInputParams();

  bool selectAnimation(const String& name);
  void getAnimations(Animation**& animations, int& numAnimations);
  Animation* getCurrentAnimation();

  void getJsonAnimations(JsonDocument& doc);

  void update();

private:

  InputParams inputParams;
  HexPanel& hexPanel;
  int ledCount;

  Animation* currentAnimation;
  Animation* animations[ANIMATION_COUNT];
};

void updateAnimation(InputParams& inputParams, LedOut leds[]);

// void animation_breath(const InputParams& inputParams, LedOut& ledOut);
// void animation_radiate(const InputParams& inputParams, LedOut& ledOut);

#endif  // __ANIMATIONS_H__
