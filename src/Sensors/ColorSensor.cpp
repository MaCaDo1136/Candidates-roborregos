#include "Sensors/ColorSensor.h"
#include "hardware/gpio.h"
#include "InterruptManager.h"

ColorSensor *ColorSensor::instances[30] = {nullptr};
ColorSensor *ColorSensor::registered_sensors[10] = {nullptr};
int ColorSensor::sensor_count = 0;
int ColorSensor::current_sensor_index = 0;
int ColorSensor::current_filter_ = 0;
repeating_timer_t ColorSensor::color_timer;

ColorSensor::ColorSensor(uint s0, uint s1, uint s2, uint s3, uint out_pin) : s0_(s0), s1_(s1), s2_(s2), s3_(s3), out_pin_(out_pin) {};

void ColorSensor::init()
{
    gpio_init(s0_);
    gpio_set_dir(s0_, GPIO_OUT);
    gpio_init(s1_);
    gpio_set_dir(s1_, GPIO_OUT);
    gpio_init(s2_);
    gpio_set_dir(s2_, GPIO_OUT);
    gpio_init(s3_);
    gpio_set_dir(s3_, GPIO_OUT);

    gpio_put(s0_, 1);
    gpio_put(s1_, 0);

    gpio_init(out_pin_);
    gpio_set_dir(out_pin_, GPIO_IN);

    instances[out_pin_] = this;
    InterruptManager::attach(out_pin_, GPIO_IRQ_EDGE_RISE, &ColorSensor::staticInterruptHandler);

    // Auto-register this sensor
    registerSensor(this);
};

void ColorSensor::handleInterrupt(uint gpio, uint32_t events)
{
    if (gpio == out_pin_)
    {
        color_count_++;
    }
};

void ColorSensor::staticInterruptHandler(uint gpio, uint32_t events)
{
    if (gpio < 30 && instances[gpio])
    {
        instances[gpio]->handleInterrupt(gpio, events);
    }
}

void ColorSensor::selectRedFilter()
{
    gpio_put(s2_, 0); // S2 = 0
    gpio_put(s3_, 0); // S3 = 0
}

void ColorSensor::selectGreenFilter()
{
    gpio_put(s2_, 1); // S2 = 1
    gpio_put(s3_, 1); // S3 = 1
}

void ColorSensor::selectBlueFilter()
{
    gpio_put(s2_, 0); // S2 = 0
    gpio_put(s3_, 1); // S3 = 1
}

void ColorSensor::startTimerManager()
{
    current_filter_ = 0;
    add_repeating_timer_ms(200, timerCallback, NULL, &color_timer);
}

bool ColorSensor::timerCallback(repeating_timer_t *rt)
{
    if (sensor_count == 0)
        return true;

    // Process all sensors in parallel
    switch (current_filter_)
    {
    case 0: // Start Red measurement for ALL sensors
        for (int i = 0; i < sensor_count; i++)
        {
            ColorSensor *sensor = registered_sensors[i];
            if (sensor)
            {
                // Store previous blue measurement (if not first cycle)
                if (current_filter_ == 0 && i == 0)
                {
                    // First measurement cycle, no previous data
                }
                else
                {
                    sensor->blue_ = sensor->getColorCount();
                }

                // Start red measurement
                sensor->selectRedFilter();
                sensor->clearColorCount();
            }
        }
        current_filter_ = 1;
        break;

    case 1: // Start Green measurement for ALL sensors
        for (int i = 0; i < sensor_count; i++)
        {
            ColorSensor *sensor = registered_sensors[i];
            if (sensor)
            {
                // Store red measurement
                sensor->red_ = sensor->getColorCount();

                // Start green measurement
                sensor->selectGreenFilter();
                sensor->clearColorCount();
            }
        }
        current_filter_ = 2;
        break;

    case 2: // Start Blue measurement for ALL sensors
        for (int i = 0; i < sensor_count; i++)
        {
            ColorSensor *sensor = registered_sensors[i];
            if (sensor)
            {
                // Store green measurement
                sensor->green_ = sensor->getColorCount();

                // Start blue measurement
                sensor->selectBlueFilter();
                sensor->clearColorCount();
            }
        }
        current_filter_ = 0; // Reset to red for next cycle
        break;
    }
    return true;
}

void ColorSensor::registerSensor(ColorSensor *sensor)
{
    if (sensor_count < 10)
    {
        registered_sensors[sensor_count++] = sensor;
    }
}
