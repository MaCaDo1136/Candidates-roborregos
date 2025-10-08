#include "Odometry/PID.h"
#include <cmath>
#include "pico/stdlib.h"

PID::PID(double p = 0, double i = 0, double d = 0) : Kp(p), Ki(i), Kd(d), integrator(0), prevError(0), outMin(-1e9), outMax(1e9), integratorMin(-1e9), integratorMax(1e9) {}

double PID::update(double error, double dt)
{
    if (dt <= 0)
    {
        return 0.0;
    }
    // Prop
    double P = Kp * error;

    integrator += error * dt;

    // Clamp
    if (integrator > integratorMax)
    {
        integrator = integratorMax;
    }
    if (integrator < integratorMin)
    {
        integrator = integratorMin;
    }

    double I = Ki * integrator;
    double D = Kd * ((error - prevError) / dt);
    double out = P + I + D;
    if (out > outMax)
    {
        out = outMax;
    }
    if (out < outMin)
    {
        out = outMin;
    }
    return out;
}