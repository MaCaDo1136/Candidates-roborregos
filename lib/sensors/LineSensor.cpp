#include <LineSensor.h>

// #include "hardware/gpio.h"
// #include "InterruptManager.h"

LineSensor::LineSensor(int pin)
{
    pin_ = pin;
};

void LineSensor::init()
{
    pinMode(pin_, INPUT);
};

void LineSensor::timerChecker()
{
}

/*
void LineSensor::handleInterrrupt()
{
    line_detected_ = digitalRead(pin_);
}
*/
/*
void LineSensor::handleInterrupt(uint gpio, uint32_t events)
{
    if (gpio == pin_)
    {
        line_detected_ = gpio_get(pin_);
    }
};

bool LineSensor::isLineDetected() const
{
    return line_detected_;
};

void LineSensor::staticInterruptHandler(uint gpio, uint32_t events)
{
    if (gpio < 30 && instances[gpio])
    {
        instances[gpio]->handleInterrupt(gpio, events);
    }
}
*/
LineSensor *LineSensor::instances[30] = {nullptr};
