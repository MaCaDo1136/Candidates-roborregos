#pragma once
#include <math.h>

struct PID
{
private:
    double Kp, Ki, Kd;
    double integrator;
    double prevError;
    double outMin, outMax;
    double integratorMin, integratorMax;

public:
    PID(double p = 0, double i = 0, double d = 0) : Kp(p), Ki(i), Kd(d), integrator(0), prevError(0), outMin(-1e9), outMax(1e9), integratorMin(-1e9), integratorMax(1e9) {}

    void setOutputLimits(double lo, double hi)
    {
        outMin = lo;
        outMax = hi;
    }

    void setIntegratorLimits(double lo, double hi)
    {
        integratorMin = lo;
        integratorMax = hi;
    }

    double reset()
    {
        integrator = 0;
        prevError = 0;
    }

    double update(double error, double dt);
};