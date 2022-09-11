#ifndef __HEX_PANEL_H__
#define __HEX_PANEL_H__

#include "LedLocations.h"

class HexPanel
{
public:
    HexPanel(int width_mm, int height_mm);

    LedOut* getLedStrip();
    int getLedCount();

private:

    int width_mm;
    int height_mm;

    LedOut ledStrip[NUM_LEDS];
};

#endif // #ifndef __HEX_PANEL_H__