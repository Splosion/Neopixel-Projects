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
#define LED_PIN 6   // Digital In (DI) of RGB Stick connected to pin 8 of the UNO

CRGB leds[NUM_LEDS]; // FastLED Library Init

int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01

RF24 radio(10, 9); // NRF24L01 used SPI pins + Pin 9 and 10 on the UNO

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01

long previousButtonPress = 0;
long currentButtonPress = 0;

int blue[3] = {54, 245, 255};
int orange[3] = {255, 45, 0};
int noColour[3] = {0, 0, 0};

int incrementRed = 0;
int incrementGreen = 0;
int incrementBlue = 0;
int currentColour[3]{0, 0, 0};

bool positiveIncrement = true;
bool fastAnimate = false;
bool buttonColour = false;
int currentLED = 0;

void setup(void)
{

    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); // Setup FastLED Library
    FastLED.clear();                                    // Clear the RGB Stick LEDs

    // Light up starting LED's
    for (int x = 0; x != NUM_LEDS; x++)
    {
        leds[x].setRGB(54, 245, 255);
    }

    FastLED.setBrightness(50);
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
        currentButtonPress = millis();
        if (ReceivedMessage[0] == 111) // Indicates switch is pressed
        {
            if (currentButtonPress - previousButtonPress >= 5000)
            {
                if (!buttonColour)
                {
                    previousButtonPress = millis();
                    fastAnimate = true;
                    buttonColour = true;
                    memcpy(currentColour, noColour, 3);
                }
                else
                {
                    previousButtonPress = millis();
                    fastAnimate = true;
                    buttonColour = false;
                    memcpy(currentColour, noColour, 3);
                }
            }
        }
        if (buttonColour && positiveIncrement)
        {
            animate(currentColour, orange);
        }
        else if (positiveIncrement)
        {
            animate(currentColour, blue);
        }
        else
        {
            animate(currentColour, noColour);
        }
        delay(10);
    }
}

void animate(int colour[3], int targetColour[3])
{
    if (incrementRed == 0 && incrementGreen == 0 && incrementBlue == 0)
    {
        incrementRed = (targetColour[0] - colour[0]) / 5;
        incrementGreen = (targetColour[1] - colour[1]) / 5;
        incrementBlue = (targetColour[2] - colour[2]) / 5;
        if (fastAnimate)
        {
            incrementRed = incrementRed * 2;
            incrementGreen = incrementGreen * 2;
            incrementBlue = incrementBlue * 2;
        }
    }
    if (positiveIncrement)
    {
        currentColour[0] = min(255, colour[0] + incrementRed);
        currentColour[1] = min(255, colour[1] + incrementGreen);
        currentColour[2] = min(255, colour[2] + incrementBlue);
        leds[currentLED].setRGB(currentColour[0], currentColour[2], currentColour[2]);
        leds[currentLED + 5].setRGB(currentColour[0], currentColour[2], currentColour[2]);
        FastLED.show();
        if (currentColour == targetColour)
        {
            positiveIncrement = false;

            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;
        }
        if (fastAnimate)
        {
            fastAnimate = false;
        }
    }
    else
    {
        currentColour[0] = colour[0] - incrementRed;
        currentColour[1] = colour[1] - incrementGreen;
        currentColour[2] = colour[2] - incrementBlue;
        leds[currentLED].setRGB(currentColour[0], currentColour[2], currentColour[2]);
        leds[currentLED + 5].setRGB(currentColour[0], currentColour[2], currentColour[2]);
        for (int x = 0; x != NUM_LEDS; x++)
        {
            leds[x].setRGB(0, 0, 0);
            FastLED.show();
        }
        if (currentColour == noColour)
        {
            positiveIncrement = true;
            incrementRed = 0;
            incrementGreen = 0;
            incrementBlue = 0;
        }
    }
}
