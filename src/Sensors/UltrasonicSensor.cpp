#include "Sensors/UltrasonicSensor.h"
#include "hardware/gpio.h"
#include "InterruptManager.h"

UltrasonicSensor::UltrasonicSensor(uint trig_pin, uint echo_pin)
{
    trig_pin_ = trig_pin;
    echo_pin_ = echo_pin;
};

void UltrasonicSensor::init()
{
    gpio_init(trig_pin_);
    gpio_set_dir(trig_pin_, GPIO_OUT);
    gpio_put(trig_pin_, 0);
    gpio_init(echo_pin_);
    gpio_set_dir(echo_pin_, GPIO_IN);
    instances[echo_pin_] = this;
    InterruptManager::attach(echo_pin_, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, &UltrasonicSensor::staticInterruptHandler);
    registerSensor(this);
};

void UltrasonicSensor::trigger()
{
    gpio_put(trig_pin_, 0);
    sleep_us(2);
    gpio_put(trig_pin_, 1);
    sleep_us(10);
    gpio_put(trig_pin_, 0);
};

float UltrasonicSensor::getDistance()
{
    if (echo_end_ > echo_start_)
    {
        uint32_t pulse_duration = echo_end_ - echo_start_;
        // Speed of sound: 343 m/s = 0.0343 cm/us
        // Distance: (pulse_duration * 0.0343) / 2
        return (pulse_duration * 0.01715f);
    }
    return -1.0f;
};

void UltrasonicSensor::handleEchoInterrupt(uint gpio, uint32_t events)
{
    if (events & GPIO_IRQ_EDGE_RISE)
    {
        echo_start_ = time_us_32();
    }
    else if (events & GPIO_IRQ_EDGE_FALL)
    {
        echo_end_ = time_us_32();
        echo_received_ = true;
    }
};

void UltrasonicSensor::staticInterruptHandler(uint gpio, uint32_t events)
{
    if (gpio < 30 && instances[gpio])
    {
        instances[gpio]->handleEchoInterrupt(gpio, events);
    }
};

UltrasonicSensor *UltrasonicSensor::instances[30] = {nullptr};
UltrasonicSensor *UltrasonicSensor::registered_sensors[10] = {nullptr};
int UltrasonicSensor::sensor_count = 0;
int UltrasonicSensor::current_sensor_index = 0;
repeating_timer_t UltrasonicSensor::ultrasonic_timer;

void UltrasonicSensor::startTimerManager()
{
    add_repeating_timer_ms(200, timerCallback, NULL, &ultrasonic_timer);
}

void UltrasonicSensor::registerSensor(UltrasonicSensor *sensor)
{
    if (sensor_count < 10)
    {
        registered_sensors[sensor_count++] = sensor;
    }
}

bool UltrasonicSensor::timerCallback(repeating_timer_t *rt)
{
    if (sensor_count == 0)
    {
        return true;
    }

    UltrasonicSensor *current_sensor = registered_sensors[current_sensor_index];

    if (current_sensor)
    {
        if (current_sensor->isEchoReceived())
        {
            current_sensor->clearEcho();
        }
        current_sensor->trigger();
    }

    // Move to next sensor
    current_sensor_index = (current_sensor_index + 1) % sensor_count;
    return true;
}