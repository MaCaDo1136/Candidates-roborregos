#include "Motors/Motor.h"
#include "hardware/pwm.h"
#include <stdio.h>

Motor::Motor(uint ena_pin, uint in1_pin, uint in2_pin)
    : ena_pin_(ena_pin), in1_pin_(in1_pin), in2_pin_(in2_pin) {}

void Motor::init()
{
    gpio_init(ena_pin_);
    gpio_set_function(ena_pin_, GPIO_FUNC_PWM);
    pwm_slice_ = pwm_gpio_to_slice_num(ena_pin_);
    pwm_set_wrap(pwm_slice_, 255);
    pwm_set_chan_level(pwm_slice_, pwm_gpio_to_channel(ena_pin_), 0);
    pwm_set_enabled(pwm_slice_, true);

    gpio_init(in1_pin_);
    gpio_set_dir(in1_pin_, GPIO_OUT);
    gpio_put(in1_pin_, 0);

    gpio_init(in2_pin_);
    gpio_set_dir(in2_pin_, GPIO_OUT);
    gpio_put(in2_pin_, 0);

    stop();
}

void Motor::setSpeed(uint8_t speed)
{
    current_speed_ = speed;
    pwm_set_chan_level(pwm_slice_, pwm_gpio_to_channel(ena_pin_), speed);
}

void Motor::forward()
{
    gpio_put(in1_pin_, 1);
    gpio_put(in2_pin_, 0);
    current_direction_ = 1;
}

void Motor::backward()
{
    gpio_put(in1_pin_, 0);
    gpio_put(in2_pin_, 1);
    current_direction_ = -1;
}

void Motor::stop()
{
    gpio_put(in1_pin_, 0);
    gpio_put(in2_pin_, 0);
    setSpeed(0);
    current_direction_ = 0;
}

void Motor::printValue()
{
    printf("Motor ENA:%u IN1:%u IN2:%u Speed:%u Dir:%d\n",
           ena_pin_, in1_pin_, in2_pin_, current_speed_, current_direction_);
}