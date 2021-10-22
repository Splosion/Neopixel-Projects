#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

#define SwitchPin1 2
#define SwitchPin2 3
#define SwitchPin3 4

int SentMessage[1] = {000}; // Used to store value before being sent through the NRF24L01
RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01

void setup(void)
{
    Serial.begin(9600);

    pinMode(SwitchPin1, INPUT_PULLUP); // input w/ pullups
    pinMode(SwitchPin2, INPUT_PULLUP);
    pinMode(SwitchPin3, INPUT_PULLUP);
    digitalWrite(SwitchPin1, HIGH);    // Set Pin HIGH
    digitalWrite(SwitchPin2, HIGH);
    digitalWrite(SwitchPin3, HIGH);
    radio.begin();               // Start the NRF24L01
    radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
}

void loop(void)
{
    if (digitalRead(SwitchPin1) == LOW)
    { // If Switch is Activated
      SentMessage[0] = 111;
      Serial.write(SentMessage[0]);
      radio.write(SentMessage, 1); // Send value through NRF24L01
      delay(250); //debounce
    }
    if (digitalRead(SwitchPin2)==LOW)
    {
      SentMessage[0] = 101;
      Serial.write(SentMessage[0]);
      radio.write(SentMessage, 1); // Send value through NRF24L01
      delay(250); //debounce
    }
    if (digitalRead(SwitchPin3)==LOW)
    {
      SentMessage[0] = 110;
      Serial.write(SentMessage[0]);
      radio.write(SentMessage, 1); // Send value through NRF24L01  
      delay(250); //debounce
    }
}
