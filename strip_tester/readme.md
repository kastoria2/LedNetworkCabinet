2022-08-07
    - Have a 'breath' animation working.  But the colors are hard-coded to a the [0] LED.
    - Don't have a mapping from index [0, 98) to the strip locations.

2022-07-28
I have 2 strips of 49 LEDs each assembled and tested.
Raw data on current draws for a strip of 49 LEDs

All current draws in mA
- Intensity 8
    - r 45.5
    - g 45.5
    - b 45.5
- Intensity 16
    - r 60.9
    - g 60.9
    - b 60.9
- Intesnity 32
    - r 91.8
    - g 91.8
    - b 91.8
- Intensity 64
    - r 156.8
    - g 156.8
    - b 156.8
- Intensity 128
    - r 220
    - g 190
    - b 180
- Intensity 255
    - r 247
    - g 214
    - b 200

2022-07-09

I was building the hexagon panel.  I hand-wired the LED by rows with 7 LEDs in them.  This program just cycles though some colors turning on each LED in turn.  Basically testing that there are no shorts or defective LEDs in the strip.