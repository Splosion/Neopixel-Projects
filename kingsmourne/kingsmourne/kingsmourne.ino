#include <FastLED.h>

#define mainPin 8
#define altPin 7
#define mainCount 27
#define altCount 15
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
CRGB mainLeds[mainCount];
CRGB altLeds[altCount];

uint8_t blue[3] = {54, 245, 255};

void setup() {
FastLED.addLeds<NEOPIXEL, mainPin>(mainLeds, mainCount);                  // Setup FastLED Library
    FastLED.addLeds<NEOPIXEL, altPin>(altLeds, altCount); // Setup FastLED Library
    FastLED.clear();                                                        // Clear the RGB Stick LEDs

    FastLED.setBrightness(85);
    FastLED.show();

}

void loop() {
  int delayTime = 100;
  changeMain(blue);
  changeAlt(blue);
delay(1000);
}
void changeMain(uint8_t colour[3])
{
    for (int i = 0; i < mainCount; i++)
        mainLeds[i].setRGB(colour[0], colour[1], colour[2]);
    FastLED.show();
}
void changeAlt(uint8_t colour[3])
{
    for (int i = 0; i < altCount; i++)
        altLeds[i].setRGB(colour[0], colour[1], colour[2]);
    FastLED.show();
}
