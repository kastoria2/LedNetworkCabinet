#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

typedef Adafruit_NeoPixel NeoPixels;

struct LedOut {
 
  uint8_t index;
  float position_mm[2];

  float distance;

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
    color(NeoPixels::Color(32, 32, 0xFF)),
    bgColor(NeoPixels::Color(0,0,0)),
    baseLocation_mm {275,300},
    speed(128)
  {}
};

InputParams inputParams;

#define BRIGHTNESS 16
#define PIN 6

#define NUM_LEDS 98
LedOut leds[NUM_LEDS] = {
LedOut {0, {25.0,43.301}, NeoPixels::Color(0,0,0)},
LedOut {1, {124.999,43.301}, NeoPixels::Color(0,0,0)},
LedOut {2, {175.0,43.301}, NeoPixels::Color(0,0,0)},
LedOut {3, {275.0,43.301}, NeoPixels::Color(0,0,0)},
LedOut {4, {325.0,43.301}, NeoPixels::Color(0,0,0)},
LedOut {5, {425.0,43.301}, NeoPixels::Color(0,0,0)},
LedOut {6, {475.0,43.301}, NeoPixels::Color(0,0,0)},
LedOut {7, {500.0,86.602}, NeoPixels::Color(0,0,0)},
LedOut {8, {400.0,86.602}, NeoPixels::Color(0,0,0)},
LedOut {9, {350.0,86.602}, NeoPixels::Color(0,0,0)},
LedOut {10, {250.0,86.602}, NeoPixels::Color(0,0,0)},
LedOut {11, {199.999,86.602}, NeoPixels::Color(0,0,0)},
LedOut {12, {100.0,86.602}, NeoPixels::Color(0,0,0)},
LedOut {13, {49.999,86.602}, NeoPixels::Color(0,0,0)},
LedOut {14, {25.0,129.903}, NeoPixels::Color(0,0,0)},
LedOut {15, {124.999,129.903}, NeoPixels::Color(0,0,0)},
LedOut {16, {175.0,129.903}, NeoPixels::Color(0,0,0)},
LedOut {17, {275.0,129.903}, NeoPixels::Color(0,0,0)},
LedOut {18, {325.0,129.903}, NeoPixels::Color(0,0,0)},
LedOut {19, {425.0,129.903}, NeoPixels::Color(0,0,0)},
LedOut {20, {475.0,129.903}, NeoPixels::Color(0,0,0)},
LedOut {21, {500.0,173.205}, NeoPixels::Color(0,0,0)},
LedOut {22, {400.0,173.205}, NeoPixels::Color(0,0,0)},
LedOut {23, {350.0,173.205}, NeoPixels::Color(0,0,0)},
LedOut {24, {250.0,173.205}, NeoPixels::Color(0,0,0)},
LedOut {25, {199.999,173.205}, NeoPixels::Color(0,0,0)},
LedOut {26, {100.0,173.205}, NeoPixels::Color(0,0,0)},
LedOut {27, {49.999,173.205}, NeoPixels::Color(0,0,0)},
LedOut {28, {25.0,216.506}, NeoPixels::Color(0,0,0)},
LedOut {29, {124.999,216.506}, NeoPixels::Color(0,0,0)},
LedOut {30, {175.0,216.506}, NeoPixels::Color(0,0,0)},
LedOut {31, {275.0,216.506}, NeoPixels::Color(0,0,0)},
LedOut {32, {325.0,216.506}, NeoPixels::Color(0,0,0)},
LedOut {33, {425.0,216.506}, NeoPixels::Color(0,0,0)},
LedOut {34, {475.0,216.506}, NeoPixels::Color(0,0,0)},
LedOut {35, {500.0,259.807}, NeoPixels::Color(0,0,0)},
LedOut {36, {400.0,259.807}, NeoPixels::Color(0,0,0)},
LedOut {37, {350.0,259.807}, NeoPixels::Color(0,0,0)},
LedOut {38, {250.0,259.807}, NeoPixels::Color(0,0,0)},
LedOut {39, {199.999,259.807}, NeoPixels::Color(0,0,0)},
LedOut {40, {100.0,259.807}, NeoPixels::Color(0,0,0)},
LedOut {41, {49.999,259.807}, NeoPixels::Color(0,0,0)},
LedOut {42, {25.0,303.108}, NeoPixels::Color(0,0,0)},
LedOut {43, {124.999,303.108}, NeoPixels::Color(0,0,0)},
LedOut {44, {175.0,303.108}, NeoPixels::Color(0,0,0)},
LedOut {45, {275.0,303.108}, NeoPixels::Color(0,0,0)},
LedOut {46, {325.0,303.108}, NeoPixels::Color(0,0,0)},
LedOut {47, {425.0,303.108}, NeoPixels::Color(0,0,0)},
LedOut {48, {475.0,303.108}, NeoPixels::Color(0,0,0)},
LedOut {49, {500.0,346.41}, NeoPixels::Color(0,0,0)},
LedOut {50, {400.0,346.41}, NeoPixels::Color(0,0,0)},
LedOut {51, {350.0,346.41}, NeoPixels::Color(0,0,0)},
LedOut {52, {250.0,346.41}, NeoPixels::Color(0,0,0)},
LedOut {53, {199.999,346.41}, NeoPixels::Color(0,0,0)},
LedOut {54, {100.0,346.41}, NeoPixels::Color(0,0,0)},
LedOut {55, {49.999,346.41}, NeoPixels::Color(0,0,0)},
LedOut {56, {25.0,389.711}, NeoPixels::Color(0,0,0)},
LedOut {57, {124.999,389.711}, NeoPixels::Color(0,0,0)},
LedOut {58, {175.0,389.711}, NeoPixels::Color(0,0,0)},
LedOut {59, {275.0,389.711}, NeoPixels::Color(0,0,0)},
LedOut {60, {325.0,389.711}, NeoPixels::Color(0,0,0)},
LedOut {61, {425.0,389.711}, NeoPixels::Color(0,0,0)},
LedOut {62, {475.0,389.711}, NeoPixels::Color(0,0,0)},
LedOut {63, {500.0,433.012}, NeoPixels::Color(0,0,0)},
LedOut {64, {400.0,433.012}, NeoPixels::Color(0,0,0)},
LedOut {65, {350.0,433.012}, NeoPixels::Color(0,0,0)},
LedOut {66, {250.0,433.012}, NeoPixels::Color(0,0,0)},
LedOut {67, {199.999,433.012}, NeoPixels::Color(0,0,0)},
LedOut {68, {100.0,433.012}, NeoPixels::Color(0,0,0)},
LedOut {69, {49.999,433.012}, NeoPixels::Color(0,0,0)},
LedOut {70, {25.0,476.313}, NeoPixels::Color(0,0,0)},
LedOut {71, {124.999,476.313}, NeoPixels::Color(0,0,0)},
LedOut {72, {175.0,476.313}, NeoPixels::Color(0,0,0)},
LedOut {73, {275.0,476.313}, NeoPixels::Color(0,0,0)},
LedOut {74, {325.0,476.313}, NeoPixels::Color(0,0,0)},
LedOut {75, {425.0,476.313}, NeoPixels::Color(0,0,0)},
LedOut {76, {475.0,476.313}, NeoPixels::Color(0,0,0)},
LedOut {77, {500.0,519.615}, NeoPixels::Color(0,0,0)},
LedOut {78, {400.0,519.615}, NeoPixels::Color(0,0,0)},
LedOut {79, {350.0,519.615}, NeoPixels::Color(0,0,0)},
LedOut {80, {250.0,519.615}, NeoPixels::Color(0,0,0)},
LedOut {81, {199.999,519.615}, NeoPixels::Color(0,0,0)},
LedOut {82, {100.0,519.615}, NeoPixels::Color(0,0,0)},
LedOut {83, {49.999,519.615}, NeoPixels::Color(0,0,0)},
LedOut {84, {25.0,562.916}, NeoPixels::Color(0,0,0)},
LedOut {85, {124.999,562.916}, NeoPixels::Color(0,0,0)},
LedOut {86, {175.0,562.916}, NeoPixels::Color(0,0,0)},
LedOut {87, {275.0,562.916}, NeoPixels::Color(0,0,0)},
LedOut {88, {325.0,562.916}, NeoPixels::Color(0,0,0)},
LedOut {89, {425.0,562.916}, NeoPixels::Color(0,0,0)},
LedOut {90, {475.0,562.916}, NeoPixels::Color(0,0,0)},
LedOut {91, {500.0,606.217}, NeoPixels::Color(0,0,0)},
LedOut {92, {400.0,606.217}, NeoPixels::Color(0,0,0)},
LedOut {93, {350.0,606.217}, NeoPixels::Color(0,0,0)},
LedOut {94, {250.0,606.217}, NeoPixels::Color(0,0,0)},
LedOut {95, {199.999,606.217}, NeoPixels::Color(0,0,0)},
LedOut {96, {100.0,606.217}, NeoPixels::Color(0,0,0)},
LedOut {97, {49.999,606.217}, NeoPixels::Color(0,0,0)}
};

NeoPixels pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

float distance(const float a[], const float b[])
{
  float xDelta = b[0] - a[0];
  float yDelta = b[1] - a[1];
  
  return sqrt(xDelta * xDelta + yDelta * yDelta);
}

float clamp(float v, float min, float max)
{
  if(v < min)
  {
    return min;
  }

  if(v > max)
  {
    return max;
  }

  return v;
}

void animation_static(const InputParams& inputParams, LedOut& ledOut)
{
  ledOut.color = inputParams.color;
}

float origin[] = {0,0};
float lowerRight[] = {625,736};
float maxDist = distance(origin, lowerRight);

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

void animation_radiate(const InputParams& inputParams, LedOut& ledOut)
{
    uint32_t CYCLE_PERIOD = 5000;

    uint32_t period = int(CYCLE_PERIOD * ((256 - inputParams.speed) / 255.0));
    uint32_t cycleTime = (inputParams.currentTime_ms - inputParams.startTime_ms) % period;
    float absPercentage = (float(cycleTime) / period);

    float ledDist = distance(inputParams.baseLocation_mm, ledOut.position_mm);
    float ledPercent = ledDist / maxDist;

    uint8_t red = (inputParams.color & 0xff0000) >> 16;
    uint8_t green = (inputParams.color & 0x00ff00) >> 8;
    uint8_t blue = inputParams.color & 0x0000ff;

    float scaleFactor = clamp((1 - abs(absPercentage - ledPercent) * 5), 0, 1.0);

    red = int(red * scaleFactor);
    green = int(green * scaleFactor);
    blue = int(blue * scaleFactor);

    ledOut.color = NeoPixels::Color(red, green, blue);
}

void updateAnimation(InputParams& inputParams, LedOut leds[])
{
  // Book keeping for animation parameters
  uint32_t currentMillis = millis();

  inputParams.deltaTime_ms = currentMillis - inputParams.currentTime_ms;
  inputParams.currentTime_ms = currentMillis;

  for(int i = 0; i < NUM_LEDS; i++)
  {
    animation_radiate(inputParams, leds[i]);
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
