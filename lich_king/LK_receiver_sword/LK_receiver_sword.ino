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

unsigned long previousButtonPress = 0;

uint8_t blue[3] = {54, 245, 255};
uint8_t orange[3] = {255, 45, 0};
uint8_t noColour[3] = {0, 0, 0};

uint8_t incrementRed = 0;
uint8_t incrementGreen = 0;
uint8_t incrementBlue = 0;
uint8_t currentColour[3]{0, 0, 0};

bool positiveIncrement = true;
bool fastAnimate = false;
bool buttonColour = false;
int currentLED = 0;

void setup(void)
{
    Serial.begin(9600);
    previousButtonPress = millis();
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); // Setup FastLED Library
    FastLED.clear();                                    // Clear the RGB Stick LEDs

    // Light up starting LED's
    for (int x = 0; x != NUM_LEDS; x++)
    {
        leds[x].setRGB(0, 0, 0);
    }

    FastLED.setBrightness(85);
    FastLED.show();

    radio.begin(); // Start the NRF24L01

    radio.openReadingPipe(1, pipe); // Get NRF24L01 ready to receive

    radio.startListening(); // Listen to see if information received

    pinMode(LED_PIN, OUTPUT); // Set RGB Stick UNO pin to an OUTPUT
}

void loop(void)
{
    unsigned long currentButtonPress = millis();
    Serial.println(currentButtonPress);
    while (radio.available())
    {
        radio.read(ReceivedMessage, 1); // Read information from the NRF24L01

        if (ReceivedMessage[0] == 111) // Indicates switch is pressed
        {
            if (currentButtonPress - previousButtonPress >= 5000)
            {
                Serial.println("Button Pressed after 5 sec");
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

            }
        }
    }
    if (positiveIncrement)
    {
        if (buttonColour)
        {
            animate(currentColour, orange);
        }
        else
        {
            animate(currentColour, blue);
        }
    }
    else
    {
        animate(currentColour, noColour);
    }
}

void animate(uint8_t colour[3], uint8_t targetColour[3])
{
    if (incrementRed == 0 && incrementGreen == 0 && incrementBlue == 0)
    {
        incrementRed = abs(max(targetColour[0], colour[0]) - min(targetColour[0], colour[0])) / 5;
        incrementGreen = abs(max(targetColour[1], colour[1]) - min(targetColour[1], colour[1])) / 5;
        incrementBlue = abs(max(targetColour[2], colour[2]) - min(targetColour[2], colour[2])) / 5;

        if (fastAnimate)
        {
            incrementRed = incrementRed * 2;
            incrementGreen = incrementGreen * 2;
            incrementBlue = incrementBlue * 2;
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
            memcpy(currentColour, noColour, 3);

            currentLED++;
        }
        if (currentLED == 5)
        {
            positiveIncrement = false;
            fastAnimate = false;
        }
    }
    else
    {
        currentColour[0] = max(0, colour[0] - incrementRed);
        currentColour[1] = max(0, colour[1] - incrementGreen);
        currentColour[2] = max(0, colour[2] - incrementBlue);
        // Serial.print("CurrentColour ");
        // Serial.print(currentColour[0]);
        // Serial.print(", ");
        // Serial.print(currentColour[1]);
        // Serial.print(", ");
        // Serial.println(currentColour[2]);
        // Serial.print("TargetColour ");
        // Serial.print(targetColour[0]);
        // Serial.print(", ");
        // Serial.print(targetColour[1]);
        // Serial.print(", ");
        // Serial.println(targetColour[2]);
        for (int x = 0; x != NUM_LEDS; x++)
        {
            leds[x].setRGB(currentColour[0], currentColour[2], currentColour[2]);
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
