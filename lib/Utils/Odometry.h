#pragma once
#include "math.h"

class Odometry
{
private:
    double x, y, theta;
    double v, omega; // The velocities to take into account
    double wheelRadius, wheelBase;
    int ticksPerRev;

    long lastLeftTicks, lastRightTicks;

public:
    Odometry(double radius, double base, int ticksRev) : x(0), y(0), theta(0), wheelRadius(radius), wheelBase(base), ticksPerRev(ticksRev), lastLeftTicks(0), lastRightTicks(0) {}

    void reset()
    {
        x = y = theta = v = omega = 0;
        lastLeftTicks = lastRightTicks = 0;
    }

    void update(long leftTicks, long rightTicks, double dt, double imuHeading = NAN);

    double getX() const { return x; }
    double getY() const { return y; }
    double getTheta() const { return theta; }
    double getLinearVelocity() const { return v; }
    double getAngularVelocity() const { return omega; }
};