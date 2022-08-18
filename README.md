# What is in this project

These are the prototype resources for the "LED Network Cabinet".  It includes
- 3D printed honeycomb / "hexagon grid" cabinet panel
  - About 2 feet x 2 feet
  - Backlit with RGB LED strip
- "Arduino" firmware source code for running the LED animations.
  - Written in the Arduino IDE.  Should support many microcontrollers that the IDE supports
  - Prototype used [Teensy 2.0](https://www.pjrc.com/store/teensy.html)
- "Arduino" project for a LED strip tester
  - I hand wired LED strips in rows of 7.  I used the strip tester to check each row as I went.
- Python & QT based firmware generator and animation tester
  - It is helpful to have the physical coordinates for the LEDs for animations.  This tool generates those coordinates so they can be pasted into the Arduino IDE
  - Also has basic LED animation simulator.  So you can test out new animations easily in Python before moving them to Arduino/C.
  
# Before you start!

This is a prototype that got too successful.  I was planning to just make 2 tiles to test how
- the tiles glued together
- the backlight looked
- the opaque hexagons fit into the tiles

It all worked out well enough that I just kept printing.  The plan was to figure everything out as I went.  So there is a good amount of cobbling used to 'finish' the project.

# Required skills/tools

- Basic crafting skills
  - gluing
  - cutting small wooden boards
  - attaching items with screws
  - etc
- 3D printer and slicer
- Soldering iron, solder, soldering skills, etc
- Use Arduino IDE to compile a sketch to a microcontroller

# Construction!!

Checkout [the wiki](https://github.com/kastoria2/LedNetworkCabinet/wiki)
