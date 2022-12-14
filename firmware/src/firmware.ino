#include <Adafruit_NeoPixel.h>

#include "LedLocations.h"
#include "HexPanel.h"
#include "Animation/Animations.h"

#include "WebService.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define BRIGHTNESS 16
#define PIN 6

HexPanel hexPanel(625, 736);
NeoPixels pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Animations animations(hexPanel, NUM_LEDS);

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  Serial.begin(9600);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(BRIGHTNESS);

  initWebService(&animations);
}

void loop() {

  // updateAnimation(inputParams, leds);
  animations.update();
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, hexPanel.getLedStrip()[i].color);
  }
  pixels.show();

  updateWebService();
}
