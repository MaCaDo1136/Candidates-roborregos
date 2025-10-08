#include "Odometry/Driver.h"

#define PI 3.1415926535897932384

Driver::Driver(double x0, double y0, double heading0, double maxLinearVel, double maxWheelRPM, Odometry *odom, double tol, double hold_time, double wheelBase)
{
    this->x0 = x0;
    this->y0 = y0;
    this->heading0 = heading0;
    pid_dist = PID(1.0, 0.2, 0.05);
    pid_head = PID(6.0, 0.0, 0.1);
    pid_dist.setIntegratorLimits(-1.0, 1.0);
    pid_dist.setOutputLimits(-maxLinearVel, maxLinearVel);
    pid_head.setOutputLimits(-1.5, 1.5);
    pid_head.setIntegratorLimits(-0.5, 0.5);
    tol_m = tol;
    this->hold_time = hold_time;
    within_since = -1;
    this->odom = odom;
    this->wheelBase = wheelBase;
}

double Driver::angleError(double target, double current)
{
    double err = fmod(target - current, 2 * PI);
    if (err > PI)
    {
        err -= 2 * PI;
    }
    if (err < -PI)
    {
        err += 2 * PI;
    }
    return err;
}

void Driver::goTo(double targetDist, double targetHeading, double dt, double &leftOut, double &rightOut)
{
    double currentTheta = odom->getTheta();
    double currentX = odom->getX();
    double currentY = odom->getY();

    double distError = targetDist - sqrt(currentX * currentX + currentY * currentY);

    double headingError = angleError(targetHeading, currentTheta);

    double v_cmd = pid_dist.update(distError, dt);
    double w_cmd = pid_head.update(headingError, dt);

    leftOut = v_cmd - (w_cmd * wheelBase / 2);
    rightOut = v_cmd + (w_cmd * wheelBase / 2);
}