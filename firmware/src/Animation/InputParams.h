#ifndef __INPUT_PARAMS_H__
#define __INPUT_PARAMS_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

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
    color(Adafruit_NeoPixel::Color(0, 0xFF, 0)),
    bgColor(Adafruit_NeoPixel::Color(0xFF,0,0xFF)),
    baseLocation_mm {625/2,736/2},
    speed(128)
  {}
};

#endif // #ifndef __INPUT_PARAMS_H__