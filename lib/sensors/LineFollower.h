#pragma once
#include <Arduino.h>
#include "LineSensor.h"
#include "Motors.h"

class LineFollower
{
private:
    LineSensor *sensorRight;
    Motor *motorLeft;
    Motor *motorRight;

    int rightThreshold; // Umbral si lo necesitas (por ahora no)
    int baseSpeed;
    int turnSpeed;

public:
    LineFollower(LineSensor *sRight, Motor *mLeft, Motor *mRight);
    void followLine();
};
