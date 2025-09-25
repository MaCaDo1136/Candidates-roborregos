#include "Sensors/EncoderSensor.h"
#include "hardware/gpio.h"
#include "InterruptManager.h"

EncoderSensor *EncoderSensor::instances[30] = {nullptr};

EncoderSensor::EncoderSensor(uint pin)
{
    pin_ = pin;
};

void EncoderSensor::init()
{
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_up(pin_);
    InterruptManager::attach(pin_, GPIO_IRQ_EDGE_FALL, &EncoderSensor::staticInterruptHandler);
};

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
