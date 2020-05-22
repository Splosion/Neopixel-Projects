/* 
Created by Yvan / https://Brainy-Bits.com

This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

// NRF24L01 Module Tutorial - Code for Receiver using Arduino UNO

//Include needed Libraries at beginning
#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "FastLED.h" // FastLED library for WS2812 RGB Stick http://fastled.io/

#define NUM_LEDS 10 // Number of leds on stick
#define NUM_LEDS_STATIC 4
#define LED_PIN 6 // Digital In (DI) of RGB Stick connected to pin 6 of the UNO
#define STATIC_LED_PIN 7

CRGB leds[NUM_LEDS]; // FastLED Library Init
CRGB staticLeds[NUM_LEDS_STATIC];

int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01

RF24 radio(10, 9); // NRF24L01 used SPI pins + Pin 9 and 10 on the UNO

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01

unsigned long previousButtonPress = 0;

uint8_t blue[3] = {54, 245, 255};
uint8_t orange[3] = {255, 45, 0};
uint8_t noColour[3] = {0, 0, 0};

uint8_t incrementRed = 0;
uint8_t incrementGreen = 0;
uint8_t incrementBlue = 0;
uint8_t currentColour[3] = {0, 0, 0};

bool positiveIncrement = true;
bool fastAnimate = false;
bool buttonColour = false;
int currentLED = 0;

int animationStepTime = 50;
int fastAnimationMultiplier = 3;

void setup(void)
{
    previousButtonPress = millis();
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);                     // Setup FastLED Library
    FastLED.addLeds<NEOPIXEL, STATIC_LED_PIN>(staticLeds, NUM_LEDS_STATIC); // Setup FastLED Library
    FastLED.clear();                                                        // Clear the RGB Stick LEDs

    FastLED.setBrightness(85);
    FastLED.show();

    radio.begin(); // Start the NRF24L01

    radio.openReadingPipe(1, pipe); // Get NRF24L01 ready to receive

    radio.startListening(); // Listen to see if information received

    pinMode(LED_PIN, OUTPUT); // Set RGB Stick UNO pin to an OUTPUT
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
                for (int i = 0; i < NUM_LEDS; i++)
                {
                    leds[i].setRGB(0, 0, 0);
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
            changeStaticLeds(orange);
        }
        else
        {
            animate(currentColour, blue);
            changeStaticLeds(blue);
        }
    }
    else
    {
        animate(currentColour, noColour);
    }
}

void changeStaticLeds(uint8_t colour[3])
{
    for (int i = 0; i < NUM_LEDS_STATIC; i++)
        staticLeds[i].setRGB(colour[0], colour[1], colour[2]);
    FastLED.show();
}

unsigned long previousAnimation = 0;

void animate(uint8_t colour[3], uint8_t targetColour[3])
{
    if (millis() < previousAnimation + animationStepTime)
        return;
    previousAnimation = millis();
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
    if (positiveIncrement)
    {
        currentColour[0] = min(targetColour[0], colour[0] + incrementRed);
        currentColour[1] = min(targetColour[1], colour[1] + incrementGreen);
        currentColour[2] = min(targetColour[2], colour[2] + incrementBlue);

        leds[currentLED].setRGB(currentColour[0], currentColour[1], currentColour[2]);
        leds[currentLED + 5].setRGB(currentColour[0], currentColour[1], currentColour[2]);
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
                previousAnimation = millis() + 1000;
                return;
            }
            memcpy(currentColour, noColour, 3);
            currentLED++;
        }
    }
    else
    {
        currentColour[0] = max(0, colour[0] - incrementRed);
        currentColour[1] = max(0, colour[1] - incrementGreen);
        currentColour[2] = max(0, colour[2] - incrementBlue);
        for (int x = 0; x != NUM_LEDS; x++)
        {
            leds[x].setRGB(currentColour[0], currentColour[1], currentColour[2]);
            FastLED.show();
        }
        if (colourCompare(currentColour, noColour) == true)
        {
            positiveIncrement = true;
            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;
        }
    }
}

boolean colourCompare(uint8_t *a, uint8_t *b)
{
    int n;

    // test each element to be the same. if not, return false
    for (n = 0; n < 3; n++)
        if (a[n] != b[n])
            return false;

    //ok, if we have not returned yet, they are equal :)
    return true;
}
