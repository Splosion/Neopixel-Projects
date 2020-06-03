#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"
#include "FastLED.h"

#define SwitchPin 4
#define shoulderPin 2
#define shoulderPin2 3
#define numShoulderLeds 60
//top 10 19 11
//sides 8 8     8 8
//bottom 14 10
//total = 96

CRGB shoulderLeds[numShoulderLeds];
CRGB shoulderLeds2[numShoulderLeds];

int SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01

RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const uint64_t pipe = 0xD6D6D6D6D6D6; // Needs to be the same for communicating between 2 NRF24L01

//colours
uint8_t yellow[3] = {255,125,0};

uint8_t blue[3] = {54, 245, 255};
uint8_t orange[3] = {255, 45, 0};
uint8_t noColour[3] = {0, 0, 0};

bool buttonColour = false;
long previousButtonPress = 0;

void setup(void)
{
    Serial.begin(9600);

    FastLED.addLeds<NEOPIXEL, shoulderPin>(shoulderLeds, numShoulderLeds);
    FastLED.addLeds<NEOPIXEL, shoulderPin2>(shoulderLeds2, numShoulderLeds);
    FastLED.clear();
    FastLED.setBrightness(100);
    FastLED.show();

    pinMode(SwitchPin, INPUT_PULLUP); // input w/ pullups
    digitalWrite(SwitchPin, HIGH);    // Set Pin HIGH

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
        if (millis() >= previousButtonPress + 1000)
        {
            previousButtonPress = millis();
            buttonColour = !buttonColour;
        }
        delay(50); //debounce
    }
    if (buttonColour)
    {
        changeLeds(orange);
    }
    else
    {
        changeLeds(yellow);
    }
}

void changeLeds(uint8_t colour[3])
{
    for (int i = 0; i < numShoulderLeds; i++)
    {
        shoulderLeds[i].setRGB(colour[0], colour[1], colour[2]);
        shoulderLeds2[i].setRGB(colour[0], colour[1], colour[2]);
        FastLED.show();
    }
}