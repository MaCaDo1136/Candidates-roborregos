#pragma once
#include <Arduino.h>
#include "SensorBase.h"

class EncoderSensor : public SensorBase
{
public:
    EncoderSensor(int pin);
    void init();
    void handleInterrupt(int gpio, uint32_t events);
    uint32_t getCount() { return count_; };
    void reset() { count_ = 0; };

    void printValue() override
    {
        printf("Encoder: %.2d cm\n", getCount());
    }

private:
    int pin_;
    volatile uint32_t count_ = 0;
    static EncoderSensor *instances[30];
    static void staticInterruptHandler(int gpio, uint32_t events);
};