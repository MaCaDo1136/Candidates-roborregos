#include "LineFollower.h"

LineFollower::LineFollower(LineSensor *sRight, Motor *mLeft, Motor *mRight)
{
    sensorRight = sRight;
    motorLeft = mLeft;
    motorRight = mRight;

    baseSpeed = 110;
    turnSpeed = 70;
}

void LineFollower::followLine()
{
    if (sensorRight->isLineDetected())
    {
        motorLeft->setSpeed(0);
        motorRight->setSpeed(0);
        delay(100);

        motorLeft->setSpeed(turnSpeed + 40);
        motorRight->setSpeed(-turnSpeed);
        delay(30);
        motorLeft->setSpeed(turnSpeed + 40);
        motorRight->setSpeed(-turnSpeed);
        delay(60);
    }
    else
    {
        motorLeft->setSpeed(0);
        motorRight->setSpeed(0);
        delay(100);

        motorLeft->setSpeed(0);
        motorRight->setSpeed(baseSpeed / 1.5);
        delay(30);
    }
}