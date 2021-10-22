#include <FastLED.h>

#define PIN 2
#define LED_COUNT 4
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
CRGB stripOne[LED_COUNT];
const byte extra=40;

uint8_t blue[3] = {54, 245, 255};
uint8_t noColour[3] = {0, 0, 0};

//increments
uint8_t incrementRed = 0;
uint8_t incrementGreen = 0;
uint8_t incrementBlue = 0;
uint8_t currentColour[3] = {0, 0, 0};

bool positiveIncrement = true;
bool fastAnimate = false;


int currentLED = 0;

int animationStepTime = 13;
int fadeStepTime = 50;
int fastAnimationMultiplier = 3;
int delaybetweenAnimations = 1000;

void setup() {
  // put your setup code here, to run once:
 FastLED.addLeds<NEOPIXEL, PIN>(stripOne, LED_COUNT);                  // Setup FastLED Library

    FastLED.clear();                                                        // Clear the RGB Stick LEDs

    FastLED.setBrightness(85);
    FastLED.show();

  }

void loop() {
if(positiveIncrement)
{
animate(currentColour,blue);
}
else
{
  animate(currentColour,noColour);
}
}

unsigned long previousAnimation = 0;


void animate(uint8_t colour[3], uint8_t targetColour[3])
{

    if (positiveIncrement)
    {
        if (millis() < previousAnimation + animationStepTime)
            return;
        previousAnimation = millis();
        SetIncrements(colour, targetColour);
        currentColour[0] = min(targetColour[0], colour[0] + incrementRed);
        currentColour[1] = min(targetColour[1], colour[1] + incrementGreen);
        currentColour[2] = min(targetColour[2], colour[2] + incrementBlue);

        stripOne[currentLED].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        stripOne[currentLED+1].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        // stripOne[currentLED + 5].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        FastLED.show();
        if (colourCompare(currentColour, targetColour) == true)
        {
            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;

            if (currentLED == 3)
            {
                positiveIncrement = false;
                fastAnimate = false;
                currentLED = 0;
                previousAnimation = millis() + delaybetweenAnimations;
                return;
            }
            memcpy(currentColour, noColour, 3);
            currentLED++;
        }
    }
    else
    {
        if (millis() < previousAnimation + fadeStepTime)
            return;
        previousAnimation = millis();
        SetIncrements(colour, targetColour);
        currentColour[0] = max(0, colour[0] - incrementRed);
        currentColour[1] = max(0, colour[1] - incrementGreen);
        currentColour[2] = max(0, colour[2] - incrementBlue);
        for (int x = 0; x != LED_COUNT; x++)
        {
            stripOne[x].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            FastLED.show();
        }
        if (colourCompare(currentColour, noColour) == true)
        {
            positiveIncrement = true;
            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;
            previousAnimation = millis() + delaybetweenAnimations;
        }
    }
}


void SetIncrements(uint8_t colour[3], uint8_t targetColour[3])
{
    if (incrementRed == 0 && incrementGreen == 0 && incrementBlue == 0)
    {
        incrementRed = abs(max(targetColour[0], colour[0]) - min(targetColour[0], colour[0])) / 10;
        incrementGreen = abs(max(targetColour[1], colour[1]) - min(targetColour[1], colour[1])) / 10;
        incrementBlue = abs(max(targetColour[2], colour[2]) - min(targetColour[2], colour[2])) / 10;

        if (fastAnimate)
        {
            incrementRed = incrementRed * fastAnimationMultiplier;
            incrementGreen = incrementGreen * fastAnimationMultiplier;
            incrementBlue = incrementBlue * fastAnimationMultiplier;
        }
    }
}
boolean colourCompare(uint8_t *a, uint8_t *b)
{
    for (int n = 0; n < 3; n++)
        if (a[n] != b[n])
            return false;
    return true;
}
