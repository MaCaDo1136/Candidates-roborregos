#pragma once
#include <Arduino.h>
#include "SensorBase.h"

class EncoderSensor : public SensorBase
{
public:
    EncoderSensor(int pin, int analog_pin);
    void init();
    uint32_t getCount() { return count_; };
    double getRotations() { return (double)count_ / 20.0; };
    int getAnalog();
    float getVelocityFromAnalog(int maxRPM);
    void reset() { count_ = 0; };

    void printValue() override
    {
        // printf("Encoder: %.2d cm\n", getCount());
    }

private:
    int pin_;
    int analog_pin_;
    bool way = true;
    volatile uint32_t count_ = 0;
    static EncoderSensor *instances[2];
    void setForward() { way = true; };
    void setBackward() { way = false; };
    void handleInterrupt();
    static void interrupt0Handler();
    static void interrupt1Handler();
};