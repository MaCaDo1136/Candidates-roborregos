#include <Motors.h>

Motor::Motor(int pinPWM, int pinIN1, int pinIN2)
    : pinPWM_(pinPWM), pinIN1_(pinIN1), pinIN2_(pinIN2)
{
}

void Motor::init()
{
    pinMode(pinPWM_, OUTPUT);
    pinMode(pinIN1_, OUTPUT);
    pinMode(pinIN2_, OUTPUT);
    stop();
}

void Motor ::setSpeed(int speed) // speed from -255 to 255
{
    if (speed > 255)
        speed = 255;
    if (speed < -255)
        speed = -255;

    if (speed >= 0)
    {
        digitalWrite(pinIN1_, HIGH);
        digitalWrite(pinIN2_, LOW);
        analogWrite(pinPWM_, speed);
    }
    else
    {
        digitalWrite(pinIN1_, LOW);
        digitalWrite(pinIN2_, HIGH);
        analogWrite(pinPWM_, -speed);
    }
}

void Motor::stop()
{
    digitalWrite(pinIN1_, LOW);
    digitalWrite(pinIN2_, LOW);
    analogWrite(pinPWM_, 0);
}