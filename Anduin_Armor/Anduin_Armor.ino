#include <Adafruit_NeoPixel.h>

#define PIN 6
#define PINTWO 5
#define LED_COUNT 28
#define LED_COUNT2 28
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip2(LED_COUNT2, PINTWO, NEO_GRB + NEO_KHZ800);
const byte extra=40;


void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  strip.begin();
     strip.setBrightness(85);  
     strip.show();
       strip2.begin();
     strip2.setBrightness(85);  
     strip2.show();

}
uint8_t blue[3] = {54, 245, 255};

void loop() {
  int delayTime = 100;
  colorWipe(54,   245,   255, delayTime);
//  ledTrail(100, 255, 25,0,0, 0,0);       // Pause; R,G,B foreground; R,G,B background

  delay(1000);
//    inverseLedTrail(100, 0, 0,0,255, 25,0);       // Pause; R,G,B foreground; R,G,B background

inverseColorWipe(54,   245,   255, delayTime);
delay(1000);
}

void colorWipe(int r, int g, int b, int wait) {
  uint32_t color = strip.Color(r,   g,   b);

  for(int i=0; i<20;i++)
  {
    strip.setPixelColor(i,color);
    strip.show();
        strip2.setPixelColor(i,color);
    strip2.show();
  }
  delay(wait);
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
       fade(10,r,g,b,i);
    strip.setPixelColor(i, color);  
    for(int j=0; j<20;j++)
  {
    strip.setPixelColor(j,color);
        strip2.setPixelColor(j,color);
    strip.show();
  }//  Set pixel's color (in RAM)
    strip2.show();
    //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  for(int i=0; i<20;i++)
  {
    strip.setPixelColor(i,color);
    strip.show();
        strip2.setPixelColor(i,color);
    strip2.show();
  }
}

void inverseColorWipe(int r, int g, int b, int wait) {
 uint32_t color = strip.Color(r,g,b);
  for(int i=strip.numPixels(); i>=10; i--) { // For each pixel in strip...
    strip.setPixelColor(i, color); 
        strip2.setPixelColor(i, color); 

        inverseFade(10,r,g,b,i);
//  Set pixel's color (in RAM)
    strip.show();   
      strip2.show();//  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
   for(int i=0; i<20;i++)
  {
    strip.setPixelColor(i,strip.Color(0,0,0));
    strip.show();
        strip2.setPixelColor(i,strip.Color(0,0,0));
    strip2.show();
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
void inverseFade(int fadeLength,int r, int g, int b, int currentPixel){
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
