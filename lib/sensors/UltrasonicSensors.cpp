#include "UltrasonicSensor.h"

UltrasonicSensor *UltrasonicSensor::instances[20] = {nullptr};
bool UltrasonicSensor::listening[20] = {false};
int UltrasonicSensor::amountSensors = 0;
bool UltrasonicSensor::stage = true;

UltrasonicSensor::UltrasonicSensor(int trig_pin, int echo_pin)
{
    trig_pin_ = trig_pin;
    echo_pin_ = echo_pin;
};

void UltrasonicSensor::init()
{
    pinMode(trig_pin_, OUTPUT);
    digitalWrite(trig_pin_, LOW);
    pinMode(echo_pin_, INPUT);
    instances[echo_pin_ % 20] = this;
    attachSensor(this);
};

void UltrasonicSensor::timerChecker()
{
    for (int i = 0; i < 20; i++)
    {
        if (!listening[i])
        {
            instances[i]->trigger();
            listening[i] = true;
        }
        else
        {
            instances[i]->handleEchoInterrupt();
        }
    }
}

void UltrasonicSensor::trigger()
{
    digitalWrite(trig_pin_, LOW);
    delay(2);
    digitalWrite(trig_pin_, HIGH);
    delay(10);
    echo_start_ = micros();
    digitalWrite(trig_pin_, LOW);
};

float UltrasonicSensor::getDistance()
{
    if (echo_received_ && echo_end_ > echo_start_)
    {
        uint32_t pulse_duration = echo_end_ - echo_start_;
        // Speed of sound: 343 m/s = 0.0343 cm/us
        // Distance: (pulse_duration * 0.0343) / 2
        echo_received_ = false;
        return (pulse_duration * 0.01715f);
    }
    return last_distance_;
};

void UltrasonicSensor::handleEchoInterrupt()
{
    bool current_echo_state = digitalRead(echo_pin_);

    if (!current_echo_state)
    {
        echo_end_ = micros();
        if (echo_end_ > echo_start_)
        {
            last_distance_ = (echo_end_ - echo_start_) * 0.01715f;
        }
        listening[sensor_id_] = false;
        return;
    }
    if ((micros() - echo_start_) > 30000)
    {
        listening[sensor_id_] = false;
        return;
    }
};

void UltrasonicSensor::attachSensor(UltrasonicSensor *sens)
{
    if (amountSensors < 20)
    {
        sens->sensor_id_ = amountSensors;
        instances[amountSensors++] = sens;
    }
}
