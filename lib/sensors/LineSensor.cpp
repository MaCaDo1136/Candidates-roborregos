#include <LineSensor.h>

// #include "hardware/gpio.h"
// #include "InterruptManager.h"

LineSensor *LineSensor::instances[10] = {nullptr};
int LineSensor::amountSensors = 0;

LineSensor::LineSensor(int pin)
{
    pin_ = pin;
    attachSensor(this);
};

void LineSensor::init()
{
    pinMode(pin_, INPUT_PULLUP);
};

void LineSensor::timerChecker()
{
    for (int i = 0; i < 10; i++)
    {
        if (instances[i])
        {
            instances[i]->handleInterrupt();
        }
    }
}

void LineSensor::handleInterrupt()
{
    line_detected_ = digitalRead(pin_);
}

bool LineSensor::isLineDetected() const
{
    return line_detected_;
};

void LineSensor::attachSensor(LineSensor *sens)
{
    instances[amountSensors] = sens;
    amountSensors++;
}
