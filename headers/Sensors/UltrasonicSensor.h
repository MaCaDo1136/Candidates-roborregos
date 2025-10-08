#pragma once
#include "pico/stdlib.h"
#include "SensorBase.h"

class UltrasonicSensor : public SensorBase
{
public:
    UltrasonicSensor(uint trig_pin, uint echo_pin);
    void init();
    void trigger();
    float getDistance();
    void handleEchoInterrupt(uint gpio, uint32_t events);

    bool isEchoReceived() const { return echo_received_; }
    void clearEcho() { echo_received_ = false; }
    static void startTimerManager();
    static void registerSensor(UltrasonicSensor *sensor);

    void printValue() override
    {
        printf("Ultras: %.2f cm\n", getDistance());
    }

private:
    uint trig_pin_;
    uint echo_pin_;
    volatile uint32_t echo_start_ = 0;
    volatile uint32_t echo_end_ = 0;
    volatile bool echo_received_ = false;
    float last_distance_ = 0;
    static void staticInterruptHandler(uint gpio, uint32_t events);
    static UltrasonicSensor *instances[30];

    static UltrasonicSensor *registered_sensors[10]; // Max 10 sensors
    static int sensor_count;
    static int current_sensor_index;
    static repeating_timer_t ultrasonic_timer;
    static bool timerCallback(repeating_timer_t *rt);
};
