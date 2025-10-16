#pragma once
#include <Arduino.h>
#include "SensorBase.h"

class UltrasonicSensor : public SensorBase
{
public:
    UltrasonicSensor(int trig_pin, int echo_pin);
    void init();
    void trigger();
    float getDistance();
    void handleEchoInterrupt();
    static void timerChecker();
    static void attachSensor(UltrasonicSensor *sens);

    void printValue() override
    {
        // printf("Ultras: %.2f cm\n", getDistance());
    }

private:
    int trig_pin_;
    int echo_pin_;
    int sensor_id_;
    volatile uint32_t echo_start_ = 0;
    volatile uint32_t echo_end_ = 0;
    volatile bool echo_received_ = false;
    float last_distance_ = 0;
    static UltrasonicSensor *instances[20]; // Max 20 sensors
    static bool listening[20];
    static int amountSensors;
    static bool stage;
};
