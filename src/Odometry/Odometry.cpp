#define _USE_MATH_DEFINES
#include "Odometry/Odometry.h"
#include "hardware/gpio.h"
#include "InterruptManager.h"
#include "math.h"

Odometry::Odometry(double radius, double base, int ticksRev) : x(0), y(0), theta(0), wheelRadius(radius), wheelBase(base), ticksPerRev(ticksRev), lastLeftTicks(0), lastRightTicks(0) {};

void Odometry::update(long leftTicks, long rightTicks, double imuHeading = NAN)
{
    long deltaLeft = leftTicks - lastLeftTicks;
    long deltaRight = rightTicks - lastRightTicks;
    lastLeftTicks = leftTicks;
    lastRightTicks = rightTicks;

    // The next is the convertion to distance
    double distPerTick = (2.0 * atan(1) * 4 * wheelRadius) / ticksPerRev;
    double dLeft = deltaLeft * distPerTick;
    double dRight = deltaRight * distPerTick;

    double dCenter = (dLeft + dRight) / 2.0;
    double dTheta = (dRight - dLeft) / wheelBase;

    x += dCenter * cos(theta + dTheta / 2.0);
    y += dCenter * sin(theta + dTheta / 2.0);
    theta += dTheta;

    if (!std::isnan(imuHeading))
    {
        theta = 0.95 * theta + 0.05 * imuHeading;
    }
};
