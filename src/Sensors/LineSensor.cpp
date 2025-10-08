#include "Sensors/LineSensor.h"
#include "hardware/gpio.h"
#include "InterruptManager.h"

LineSensor::LineSensor(uint pin)
{
    pin_ = pin;
};

void LineSensor::init()
{
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    instances[pin_] = this;
    InterruptManager::attach(pin_, GPIO_IRQ_EDGE_RISE, &LineSensor::staticInterruptHandler);
};

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

LineSensor *LineSensor::instances[30] = {nullptr};
