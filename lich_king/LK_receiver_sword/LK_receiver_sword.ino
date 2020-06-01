#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "FastLED.h" // FastLED library for WS2812 RGB Stick http://fastled.io/

//led parameters
#define numRuneLeds 10
#define numSkullLeds 4
#define runePin 6
#define skullPin 8

//initialise strips
CRGB runeLeds[numRuneLeds];
CRGB skullLeds[numSkullLeds];

int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01

RF24 radio(10, 9);

const uint64_t channel = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01

unsigned long previousButtonPress = 0;

//colours
uint8_t blue[3] = {54, 245, 255};
uint8_t orange[3] = {255, 45, 0};
uint8_t noColour[3] = {0, 0, 0};

//increments
uint8_t incrementRed = 0;
uint8_t incrementGreen = 0;
uint8_t incrementBlue = 0;
uint8_t currentColour[3] = {0, 0, 0};

bool positiveIncrement = true;
bool fastAnimate = false;
bool buttonColour = false;
int currentLED = 0;

int animationStepTime = 13;
int fadeStepTime = 50;
int fastAnimationMultiplier = 3;
int delaybetweenAnimations = 1000;
int numLedsInSingleRuneStrip = 13;

void setup(void)
{
    previousButtonPress = millis();
    FastLED.addLeds<NEOPIXEL, runePin>(runeLeds, numRuneLeds);    // Setup FastLED Library
    FastLED.addLeds<NEOPIXEL, skullPin>(skullLeds, numSkullLeds); // Setup FastLED Library
    FastLED.clear();                                              // Clear the RGB Stick LEDs

    FastLED.setBrightness(85);
    FastLED.show();

    radio.begin(); // Start the NRF24L01

    radio.openReadingPipe(1, channel); // Get NRF24L01 ready to receive

    radio.startListening(); // Listen to see if information received

    pinMode(runePin, OUTPUT); // Set RGB Stick UNO pin to an OUTPUT
}

void loop(void)
{

    while (radio.available())
    {
        radio.read(ReceivedMessage, 1); // Read information from the NRF24L01
        if (ReceivedMessage[0] == 111)  // Indicates switch is pressed
        {
            if (millis() >= previousButtonPress + 1000)
            {
                previousButtonPress = millis();
                fastAnimate = true;
                buttonColour = !buttonColour;

                //reset animation
                positiveIncrement = true;
                memcpy(currentColour, noColour, 3);
                incrementRed = 0;
                incrementBlue = 0;
                incrementGreen = 0;
                currentLED = 0;
                for (int i = 0; i < numRuneLeds; i++)
                {
                    runeLeds[i].setRGB(0, 0, 0);
                    FastLED.show();
                }

                delay(50); //debounce
            }
        }
    }
    if (positiveIncrement)
    {
        if (buttonColour)
        {
            animate(currentColour, orange);
            changeSkullLeds(orange);
        }
        else
        {
            animate(currentColour, blue);
            changeSkullLeds(blue);
        }
    }
    else
    {
        animate(currentColour, noColour);
    }
}

void changeSkullLeds(uint8_t colour[3])
{
    for (int i = 0; i < numSkullLeds; i++)
        skullLeds[i].setRGB(colour[0], colour[1], colour[2]);
    FastLED.show();
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

        //firstRune = 3
        //secondRune = 1
        //thirdRune = 2
        //fourthRune = 4
        //fifthRune = 3

        switch (currentLED) //this is a little too specific to frostmourne, optimise? also DRY
        {
        case 0:
        case 1:
        case 2:
            for (int i = currentLED; i < 2; i++)
            {
                runeLeds[i].setRGB(currentColour[0], currentColour[1], currentColour[2]);
                runeLeds[i + numLedsInSingleRuneStrip].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            }
            currentLED = 3;
            break;
        case 3:
            runeLeds[currentLED].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            runeLeds[currentLED + numLedsInSingleRuneStrip].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            currentLED = 4;
            break;
        case 4:
        case 5:
            for (int i = currentLED; i < 5; i++)
            {
                runeLeds[i].setRGB(currentColour[0], currentColour[1], currentColour[2]);
                runeLeds[i + numLedsInSingleRuneStrip].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            }
            currentLED = 36 break;
        case 6:
        case 7:
        case 8:
        case 9:
            for (int i = currentLED; i < 9; i++)
            {
                runeLeds[i].setRGB(currentColour[0], currentColour[1], currentColour[2]);
                runeLeds[i + numLedsInSingleRuneStrip].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            }
            currentLED = 10;
            break;
        case 10:
        case 11:
        case 12:
            for (int i = currentLED; i < 12; i++)
            {
                runeLeds[i].setRGB(currentColour[0], currentColour[1], currentColour[2]);
                runeLeds[i + numLedsInSingleRuneStrip].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            }
            currentLED = 12;
            break;
        }
        FastLED.show();
        if (colourCompare(currentColour, targetColour) == true)
        {
            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;

            if (currentLED == 12) //end of this rune strip
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
        for (int x = 0; x != numRuneLeds; x++)
        {
            runeLeds[x].setRGB(currentColour[0], currentColour[1], currentColour[2]);
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
