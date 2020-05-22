/*
Created by Yvan / https://Brainy-Bits.com

This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

// NRF24L01 Module Tutorial - Code for Transmitter using Arduino NANO

//Include needed Libraries at beginning
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "FastLED.h"

#define SwitchPin 3 // Arcade switch is connected to Pin 8 on NANO
#define helmPin 7
#define numHelmLeds 2

CRGB helmLeds[numHelmLeds];

int SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01

RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01

//colours
uint8_t blue[3] = {54, 245, 255};
uint8_t orange[3] = {255, 45, 0};
uint8_t noColour[3] = {0, 0, 0};

bool buttonColour = false;

void setup(void)
{
    Serial.begin(9600);

    FastLED.addLeds<NEOPIXEL, helmPin>(helmLeds, numHelmLeds);
    FastLED.clear();
    FastLED.setBrightness(85);
    FastLED.show();

    pinMode(SwitchPin, INPUT_PULLUP); // Define the arcade switch NANO pin as an Input using Internal Pullups
    digitalWrite(SwitchPin, HIGH);    // Set Pin to HIGH at beginning

    radio.begin();               // Start the NRF24L01
    radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
}

void loop(void)
{

    if (digitalRead(SwitchPin) == LOW)
    { // If Switch is Activated
        SentMessage[0] = 111;
        Serial.write("1");
        radio.write(SentMessage, 1); // Send value through NRF24L01

        buttonColour = true;
        delay(50); //debounce
    }
    if (buttonColour)
    {
        changeLeds(orange);
    }
    else
    {
        changeLeds(blue);
    }
}

void changeLeds(uint8_t colour[3])
{
    for (int i = 0; i < numHelmLeds; i++)
    {
        helmLeds[i].setRGB(colour[0], colour[1], colour[2]);
        FastLED.show();
    }
}