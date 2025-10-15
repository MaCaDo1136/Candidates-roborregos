#pragma once
#include "SensorBase.h"
#include <Arduino.h>

class LineSensor : public SensorBase
{
public:
    LineSensor(int pin);
    void init();
    static void timerChecker();
    static void attachSensor(LineSensor *sens);
    void handleInterrupt();
    bool isLineDetected() const;

    void printValue() override
    {
        printf("Line: %d cm\n", isLineDetected());
    }

private:
    int pin_;
    volatile bool line_detected_ = false;
    static int amountSensors;
    static LineSensor *instances[10];
};