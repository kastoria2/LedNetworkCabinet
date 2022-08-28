#include <Adafruit_NeoPixel.h>
typedef Adafruit_NeoPixel NeoPixels;

#include "LedLocations.h"
#include "Animations.h"

#include "WebService.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define BRIGHTNESS 16
#define PIN 6

NeoPixels pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  Serial.begin(9600);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(BRIGHTNESS);

  initWebService();
}

void loop() {

  updateAnimation(inputParams, leds);
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, leds[i].color);
  }
  pixels.show();

  updateWebService();
}