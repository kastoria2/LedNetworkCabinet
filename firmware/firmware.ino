#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

typedef Adafruit_NeoPixel NeoPixels;

struct LedOut {
 
  uint8_t index;
  float position_um[2];

  uint32_t color;
};

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
    color(NeoPixels::Color(0xFF, 0, 0xFF)),
    bgColor(NeoPixels::Color(0,0,0)),
    baseLocation_mm {0,0},
    speed(128)
  {}
};

InputParams inputParams;

#define BRIGHTNESS 16
#define PIN 6
#define NUM_LEDS 98

NeoPixels pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

LedOut leds[NUM_LEDS] = {
  LedOut {0, {0,0}, NeoPixels::Color(0,0,0)},
  LedOut {1, {100,0}, NeoPixels::Color(0,0,0)},
  LedOut {2, {200,0}, NeoPixels::Color(0,0,0)},
  LedOut {3, {300,0}, NeoPixels::Color(0,0,0)},
  LedOut {4, {400,0}, NeoPixels::Color(0,0,0)},
  LedOut {5, {500,0}, NeoPixels::Color(0,0,0)},
  LedOut {6, {600,0}, NeoPixels::Color(0,0,0)},
  LedOut {7, {700,0}, NeoPixels::Color(0,0,0)},
  LedOut {8, {800,0}, NeoPixels::Color(0,0,0)},
  LedOut {9, {900,0}, NeoPixels::Color(0,0,0)},
  LedOut {10, {1000,0}, NeoPixels::Color(0,0,0)},
  LedOut {11, {1100,0}, NeoPixels::Color(0,0,0)},
  LedOut {12, {1200,0}, NeoPixels::Color(0,0,0)},
  LedOut {13, {1300,0}, NeoPixels::Color(0,0,0)},
  LedOut {14, {1400,0}, NeoPixels::Color(0,0,0)},
  LedOut {15, {1500,0}, NeoPixels::Color(0,0,0)}
};

void animation_static(const InputParams& inputParams, LedOut& led)
{
  led.color = inputParams.color;
}

void animation_breath(const InputParams& inputParams, LedOut& ledOut)
{
    uint32_t CYCLE_PERIOD = 6000;

    // Max period is 5s for a breath cycle.
    uint32_t period = int(CYCLE_PERIOD * ((256 - inputParams.speed) / 255.0));

    uint32_t cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period;

    float absPercentage = (abs((period/2.0) - cycleTime) / period) * 2;

    uint8_t red = (inputParams.color & 0xff0000) >> 16;
    uint8_t green = (inputParams.color & 0x00ff00) >> 8;
    uint8_t blue = inputParams.color & 0x0000ff;

    red = int(red * absPercentage);
    green = int(green * absPercentage);
    blue = int(blue * absPercentage);

    ledOut.color = NeoPixels::Color(red, green,blue);
}

void updateAnimation(InputParams& inputParams, LedOut leds[])
{
  // Book keeping for animation parameters
  uint32_t currentMillis = millis();

  inputParams.deltaTime_ms = currentMillis - inputParams.currentTime_ms;
  inputParams.currentTime_ms = currentMillis;

  for(int i = 0; i < NUM_LEDS; i++)
  {
    animation_breath(inputParams, leds[i]);
  }
}

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(BRIGHTNESS);
}

void loop() {

  updateAnimation(inputParams, leds);
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, leds[i].color);
  }
  pixels.show();
}
