#ifndef __ANIMATIONS_H__
#define __ANIMATIONS_H__

#include <Arduino.h>
#include "LedLocations.h"

struct InputParams {

  uint32_t startTime_ms;
  uint32_t currentTime_ms;
  uint32_t deltaTime_ms;

  uint32_t color;
  uint32_t bgColor;

  float baseLocation_mm[2];

  uint8_t speed;

  InputParams() : 
    startTime_ms(0),
    currentTime_ms(0),
    deltaTime_ms(0),
    color(NeoPixels::Color(32, 32, 0xFF)),
    bgColor(NeoPixels::Color(0,0,0)),
    baseLocation_mm {275,300},
    speed(128)
  {}
};

extern InputParams inputParams;

float distance(const float a[], const float b[]);
float clamp(float v, float min, float max);

void updateAnimation(InputParams& inputParams, LedOut leds[]);

void animation_static(const InputParams& inputParams, LedOut& ledOut);
void animation_breath(const InputParams& inputParams, LedOut& ledOut);
void animation_radiate(const InputParams& inputParams, LedOut& ledOut);

#endif  // __ANIMATIONS_H__