#pragma once
#include "pico/stdlib.h"
#include "SensorBase.h"

class LineSensor : public SensorBase
{
public:
    LineSensor(uint pin);
    void init();
    void handleInterrupt(uint gpio, uint32_t events);
    bool isLineDetected() const;

    void printValue() override
    {
        printf("Line: %d cm\n", isLineDetected());
    }

private:
    uint pin_;
    volatile bool line_detected_ = false;
    static LineSensor *instances[30];
    static void staticInterruptHandler(uint gpio, uint32_t events);
};
