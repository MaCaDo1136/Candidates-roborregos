#include <EncoderSensor.h>

EncoderSensor *EncoderSensor::instances[2] = {nullptr};

EncoderSensor::EncoderSensor(int pin)
{
    pin_ = pin;
};

void EncoderSensor::init()
{
    pinMode(pin_, INPUT);
    // Checking that the encoder work correctly
    instances[pin_] = this;
    attachInterrupt(digitalPinToInterrupt(pin_), staticInterruptHandler, CHANGE);
};

/*
void EncoderSensor::handleInterrupt(uint gpio, uint32_t events)
{
    count_++;
};

void EncoderSensor::staticInterruptHandler(uint gpio, uint32_t events)
{
    if (gpio < 30 && instances[gpio])
    {
        instances[gpio]->handleInterrupt(gpio, events);
    }
};
*/
