#pragma once
#include "pico/stdlib.h"
#include "Sensors/SensorBase.h"

class Motor : public SensorBase
{
public:
    Motor(uint ena_pin, uint in1_pin, uint in2_pin);
    void init();
    void setSpeed(uint8_t speed); // 0-255 for PWM
    void forward();
    void backward();
    void stop();

    // For debugging/printing
    void printValue();

private:
    uint ena_pin_, in1_pin_, in2_pin_;
    uint pwm_slice_;
    uint16_t current_speed_ = 0;
    int current_direction_ = 0; // 1=forward, -1=backward, 0=stop
};