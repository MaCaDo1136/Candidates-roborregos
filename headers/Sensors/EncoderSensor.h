#pragma once
#include "pico/stdlib.h"
#include "SensorBase.h"

class EncoderSensor : public SensorBase
{
public:
    EncoderSensor(uint pin);
    void init();
    void handleInterrupt(uint gpio, uint32_t events);
    uint32_t getCount() { return count_; };
    void reset() { count_ = 0; };

    void printValue() override
    {
        printf("Encoder: %.2d cm\n", getCount());
    }

private:
    uint pin_;
    volatile uint32_t count_ = 0;
    static EncoderSensor *instances[30];
    static void staticInterruptHandler(uint gpio, uint32_t events);
};
