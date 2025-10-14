#include "MyLED.h"

MyLED::MyLED(int pin)
{
    _ledPin = pin;
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
}

// turnOn method implementation
void MyLED::turnOn()
{
    digitalWrite(_ledPin, HIGH);
}

// turnOff method implementation
void MyLED::turnOff()
{
    digitalWrite(_ledPin, LOW);
}

// toggle method implementation
void MyLED::toggle()
{
    digitalWrite(_ledPin, !digitalRead(_ledPin)); // Read current state and set to opposite
}