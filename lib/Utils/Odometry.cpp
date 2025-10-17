#define _USE_MATH_DEFINES
#include <Odometry.h>
#include "math.h"
void Odometry::update(long leftTicks, long rightTicks, double dt, double dHeading)
{
    long deltaLeft = leftTicks - lastLeftTicks;
    long deltaRight = rightTicks - lastRightTicks;
    lastLeftTicks = leftTicks;
    lastRightTicks = rightTicks;

    // The next is the convertion to distance
    double distPerTick = (2.0 * M_PI * wheelRadius) / ticksPerRev;
    double dLeft = deltaLeft * distPerTick;
    double dRight = deltaRight * distPerTick;

    double dCenter = (dLeft + dRight) / 2.0;
    double dTheta = (dRight - dLeft) / wheelBase;

    dTheta = (dTheta) / 1.1;

    x += dCenter * cos(theta + dTheta / 2.0);
    y += dCenter * sin(theta + dTheta / 2.0);
    theta += dTheta;
};