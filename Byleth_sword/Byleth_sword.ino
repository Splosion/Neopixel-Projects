#include <Adafruit_NeoPixel.h>

#define PIN 6
#define LED_COUNT 40
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
const byte extra=40;


void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  strip.begin();
     strip.setBrightness(85);  
     strip.show();

}

void loop() {
  int delayTime = 100;
  colorWipe(255,   25,   0, delayTime);
//  ledTrail(100, 255, 25,0,0, 0,0);       // Pause; R,G,B foreground; R,G,B background

  delay(1000);
//    inverseLedTrail(100, 0, 0,0,255, 25,0);       // Pause; R,G,B foreground; R,G,B background

inverseColorWipe(255,   25,   0, delayTime);
delay(1000);
}

void colorWipe(int r, int g, int b, int wait) {
  uint32_t color = strip.Color(r,   g,   b);

  for(int i=0; i<10;i++)
  {
    strip.setPixelColor(i,color);
    strip.show();
  }
  delay(wait);
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
       fade(10,r,g,b,i);
    strip.setPixelColor(i, color);  
    for(int j=0; j<10;j++)
  {
    strip.setPixelColor(j,color);
  }//  Set pixel's color (in RAM)
    strip.show();
    //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  for(int i=0; i<10;i++)
  {
    strip.setPixelColor(i,color);
    strip.show();
  }
}

void inverseColorWipe(int r, int g, int b, int wait) {
 uint32_t color = strip.Color(r,g,b);
  for(int i=strip.numPixels(); i>=0; i--) { // For each pixel in strip...
    strip.setPixelColor(i, color); 
        inverseFade(10,r,g,b,i, 10);
//  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
   for(int i=0; i<10;i++)
  {
    strip.setPixelColor(i,strip.Color(0,0,0));
    strip.show();
  }
}

void fade(int fadeLength,int r, int g, int b,int currentPixel) {
for (int ahead=0;ahead <fadeLength;ahead++){
  if (currentPixel+ahead <= strip.numPixels()-1) {
    double divisor = (((double)fadeLength-ahead)/(fadeLength+1));
//    Serial.println(divisor);
    strip.setPixelColor(currentPixel+ahead,strip.Color(r*divisor,g*divisor,b*divisor));
    }
  }
}
void inverseFade(int fadeLength,int r, int g, int b, int currentPixel, int end){
    int diff = strip.numPixels()-currentPixel;
    if (diff<fadeLength){
        fadeLength=diff;
        }
    for (int ahead=0;ahead <fadeLength+1;ahead++){
  if (currentPixel+ahead <= strip.numPixels()-1) {
    double divisor = (((double)fadeLength-ahead)/(fadeLength+1));
//    Serial.println(divisor);
    strip.setPixelColor(currentPixel+ahead,strip.Color(r*divisor,g*divisor,b*divisor));
    }
    }
}
