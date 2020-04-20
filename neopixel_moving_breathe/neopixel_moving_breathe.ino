#include <Adafruit_NeoPixel.h>

#define PIN 6
#define numPix 14

Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN);


const int buttonPin = 5;
int buttonState = 0;
void setup() {
  
  strip.begin();
  
  strip.setBrightness(85);  
  
  strip.show();
  
}

void loop() {

colorWipe(strip.Color(54,   255,   245), 125);

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
def fade(self, length,color, currentPixel):
        for ahead in range(0,length,1):
            if currentPixel+ahead <= self.pixels.numPixels()-1:
                divisor = ((length-ahead)/(length+1))
                self.pixels.setPixelColor(currentPixel+ahead%self.pixels.numPixels(),self.pixels.Color(color[0]*divisor,color[1]*divisor,color[2]*divisor))
    def inverseFade(self, length,color, currentPixel, end):
        diff = self.pixels.numPixels()-currentPixel
        if self.pixels.numPixels()-currentPixel<length:
            length=diff
        for ahead in range(1,length+1,1):
            divisor = ((ahead)/(length+1))
            print(diff)
            print(currentPixel)
            print(divisor)
            print((currentPixel+self.pixels.numPixels()-ahead)%self.pixels.numPixels())
            if (currentPixel+self.pixels.numPixels()-ahead)%self.pixels.numPixels() >=10:
                self.pixels.setPixelColor((currentPixel+self.pixels.numPixels()-ahead)%self.pixels.numPixels(),self.pixels.Color(color[0]*divisor,color[1]*divisor,color[2]*divisor))
