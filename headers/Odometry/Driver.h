#pragma once
#include <math.h>
#include "Odometry/PID.h"
#include "Odometry/Odometry.h"

class Driver
{
private:
    double x0;
    double y0;
    double heading0;
    PID pid_dist;
    PID pid_head;
    double wheelBase;
    Odometry *odom;

    // stopping criteria
    double tol_m; // tolerance
    double hold_time;
    double within_since;

public:
    Driver(double x0, double y0, double heading0, double maxLinearVel, double maxWheelRPM, Odometry *odom, double tol, double hold_time, double wheelBase);

    double angleError(double target, double current);

    void goTo(double targetDist, double targetHeading, double dt, double &leftOut, double &rightOut);
};