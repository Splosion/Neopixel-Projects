#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "FastLED.h" // FastLED library for WS2812 RGB Stick http://fastled.io/

//led parameters
#define numLedsOne 10
#define numLedsTwo 4
#define pinOne 2
#define pinTwo 3


//initialise strips
CRGB stripOne[numLedsOne];
CRGB stripTwo[numLedsTwo];

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

void setup(void)
{
    previousButtonPress = millis();
    FastLED.addLeds<NEOPIXEL, pinOne>(stripOne, numLedsOne);                  // Setup FastLED Library
    FastLED.clear();                                                        // Clear the RGB Stick LEDs

    FastLED.setBrightness(85);
    FastLED.show();

    radio.begin(); // Start the NRF24L01

    radio.openReadingPipe(1, channel); // Get NRF24L01 ready to receive

    radio.startListening(); // Listen to see if information received

    pinMode(pinOne, OUTPUT); // Set RGB Stick UNO pin to an OUTPUT
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
                for (int i = 0; i < numLedsOne; i++)
                {
                    stripOne[i].setRGB(0, 0, 0);
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
            changestripTwo(orange);
        }
        else
        {
            animate(currentColour, blue);
            changestripTwo(blue);
        }
    }
    else
    {
        animate(currentColour, noColour);
    }
}

void changestripTwo(uint8_t colour[3])
{
    for (int i = 0; i < numLedsTwo; i++)
        stripTwo[i].setRGB(colour[0], colour[1], colour[2]);
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

        stripOne[currentLED].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        stripOne[currentLED+1].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        // stripOne[currentLED + 5].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        FastLED.show();
        if (colourCompare(currentColour, targetColour) == true)
        {
            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;

            if (currentLED == 4)
            {
                positiveIncrement = false;
                fastAnimate = false;
                currentLED = 0;
                previousAnimation = millis() + delaybetweenAnimations;
                return;
            }
            memcpy(currentColour, noColour, 3);
            currentLED+=2;
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
        for (int x = 0; x != numLedsOne; x++)
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
