#include <Adafruit_NeoPixel.h>

#define PIN 6
#define PIN2 7
#define PIN3 8
#define numPix 14
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN);

Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(3,PIN2);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(4,PIN3);

const int buttonPin = 5;
int buttonState = 0;
void setup() {
  
  strip.begin();
  
  strip2.begin();
  strip3.begin();
  strip.setBrightness(85);  // Lower brightness and save eyeballs!
  
  strip2.setBrightness(85);
  strip3.setBrightness(85);
  strip.show(); // Initialize all pixels to 'off'
  
  
  strip2.setPixelColor(0,0,255,255);
  strip2.setPixelColor(1,0,255,255);
  strip2.setPixelColor(2,0,255,255);
  strip2.show();
  strip3.setPixelColor(0,0,255,255);
  strip3.setPixelColor(1,0,255,255);
  strip3.setPixelColor(2,0,255,255);
  strip3.setPixelColor(3,0,255,255);
  strip3.show();
}

void loop() {
//Written by: Jason Yandell
//
//int TOTAL_LEDS = 60;
//float MaximumBrightness = 255;
//float SpeedFactor = 0.008; // I don't actually know what would look good
//float StepDelay = 5; // ms for a step delay on the lights
//
//// Make the lights breathe
//for (int i = 0; i < 65535; i++) {
//// Intensity will go from 10 - MaximumBrightness in a "breathing" manner
//float intensity = MaximumBrightness /2.0 * (1.0 + sin(SpeedFactor * i));
//strip.setBrightness(intensity);
//// Now set every LED to that color
//for (int ledNumber=0; ledNumber<TOTAL_LEDS; ledNumber++) {
//strip.setPixelColor(ledNumber, 0, 0, 255);
//}
//
//strip.show();
////Wait a bit before continuing to breathe
//delay(StepDelay);

  //ledTrail(100, 6, 255,255,0, 0,200);       // Pause; R,G,B foreground; R,G,B background
colorWipe(strip.Color(0,   255,   255), 125);

delay(1500);
darken();
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
void darken() {
  
  uint16_t i, j;

  for (j = 255; j > 0; j--) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, j, j);
    }
    strip.show();
    delay(10);
    
  }
  delay(1500);
}

const byte extra = 60;  // how many ahead, and behind, to light up

// Pause = delay between transitions
// Rf, Gf, Bf = RGB "Foreground" values
// Rb, Gb, Bb = RGB "Background" values
void ledTrail(int pause, byte Rf, byte Gf, byte Bf, byte Rb, byte Gb, byte Bb) { 

  int tmpR, tmpG, tmpB;         // Temp values

  for (int i=0; i<numPix; i++) {
    strip.setPixelColor(i,Rf,Gf,Bf);

    // now go "extra" pixels ahead and behind
    for (int s = 1; s < extra + 1; s++)
      {
      tmpR = (Rf * (extra - s)) / extra;     // Multiply first to avoid truncation errors  
      tmpG = (Gf * (extra - s)) / extra;
      tmpB = (Bf * (extra - s)) / extra;
      strip.setPixelColor((i + s) % numPix,tmpR,tmpG,tmpB);
      strip.setPixelColor((i + numPix - s) % numPix,tmpR,tmpG,tmpB);
      }

    strip.show();
    delay(pause);
  }
}
