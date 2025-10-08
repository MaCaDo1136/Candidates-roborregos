#pragma once
#include "pico/stdlib.h"
#include "SensorBase.h"

class ColorSensor : public SensorBase
{
public:
    ColorSensor(uint s0, uint s1, uint s2, uint s3, uint out_pin);
    void init();
    void handleInterrupt(uint gpio, uint32_t events);
    uint32_t getRed() const { return red_; }
    uint32_t getGreen() const { return green_; }
    uint32_t getBlue() const { return blue_; }
    void selectRedFilter();
    void selectGreenFilter();
    void selectBlueFilter();

    static void startTimerManager();
    static void registerSensor(ColorSensor *sensor);
    void printValue() override
    {
        printf("Color: R:%lu G:%lu B:%lu\n", getRed(), getGreen(), getBlue());
    }

private:
    uint s0_, s1_, s2_, s3_, out_pin_;
    volatile uint32_t color_count_ = 0;
    volatile uint32_t red_ = 0, green_ = 0, blue_ = 0;
    static int current_filter_;

    void clearColorCount() { color_count_ = 0; }
    uint32_t getColorCount() const { return color_count_; }

    static ColorSensor *instances[30];
    static void staticInterruptHandler(uint gpio, uint32_t events);

    static ColorSensor *registered_sensors[10]; // Max 10 sensors
    static int sensor_count;
    static int current_sensor_index;
    static repeating_timer_t color_timer;
    static bool timerCallback(repeating_timer_t *rt);
};
