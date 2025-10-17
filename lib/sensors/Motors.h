#pragma once
#include "SensorBase.h"
#include <Arduino.h>

class Motor
{
public:
    Motor(int pinPWM, int pinIN1, int pinIN2);
    void init();
    void setSpeed(int speed); // speed from -255 to 255
    void stop();

private:
    int pinPWM_;
    int pinIN1_;
    int pinIN2_;
};
