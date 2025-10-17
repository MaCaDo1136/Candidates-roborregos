#pragma once
#include <Arduino.h>
#include "LineSensor.h"
#include "Motors.h"

class LineFollower
{
private:
    LineSensor *line_left;
    LineSensor *line_right;
    Motor *motor_left;
    Motor *motor_right;

    bool invertLeft;
    bool invertRight;

    int baseSpeed;
    int turnSpeed;
    int spinSpeed;
    int debounceReads;
    unsigned long lostTimeoutMs;

    enum LastDir
    {
        DIR_UNKNOWN = 0,
        DIR_LEFT = -1,
        DIR_RIGHT = 1
    };
    LastDir lastSeen;
    int leftCounter;
    int rightCounter;
    unsigned long lastSeenTime;
    bool alternateSpinDir;

public:
    LineFollower(LineSensor *left, LineSensor *right, Motor *mLeft, Motor *mRight);

    void setSpeeds(int baseSpd, int turnSpd, int spinSpd);
    void setDebounce(int reads);
    void setTimeout(unsigned long timeout);
    void init();
    void update(); // llamada principal en loop()
    void setInverted(bool leftInv, bool rightInv);
};
