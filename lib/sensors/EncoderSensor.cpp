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
    instances[pin_ % 2] = this;
    if (pin_ % 2 == 0)
    {
        attachInterrupt(digitalPinToInterrupt(pin_), EncoderSensor::interrupt0Handler, RISING);
    }
    else if (pin_ % 2 == 1)
    {
        attachInterrupt(digitalPinToInterrupt(pin_), EncoderSensor::interrupt1Handler, RISING);
    }
};

void EncoderSensor::interrupt0Handler()
{
    if (instances[0])
    {
        instances[0]->handleInterrupt();
    }
}

void EncoderSensor::interrupt1Handler()
{
    if (instances[1])
    {
        instances[1]->handleInterrupt();
    }
}

void EncoderSensor::handleInterrupt()
{
    if (way)
    {
        count_++;
    }
    else
    {
        count_--;
    }
}

/*
void EncoderSensor::staticInterruptHandler(uint gpio, uint32_t events)
{
    if (gpio < 30 && instances[gpio])
    {
        instances[gpio]->handleInterrupt(gpio, events);
    }
};
*/
