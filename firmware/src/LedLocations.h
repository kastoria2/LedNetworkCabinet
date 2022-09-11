#ifndef __LED_LOCATIONS_H__
#define __LED_LOCATIONS_H__

#include <Adafruit_NeoPixel.h>
typedef Adafruit_NeoPixel NeoPixels;

struct LedOut {
 
  uint8_t index;
  float position_mm[2];

  uint32_t color;

  float distance;
};

#define NUM_LEDS 98

#endif // __LED_LOCATIONS_H__