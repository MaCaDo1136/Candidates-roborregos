#include "LineFollower.h"

LineFollower::LineFollower(LineSensor *left, LineSensor *right, Motor *mLeft, Motor *mRight)
{
    line_left = left;
    line_right = right;
    motor_left = mLeft;
    motor_right = mRight;

    baseSpeed = 100;
    turnSpeed = 60;
    spinSpeed = 80;
    debounceReads = 3;
    lostTimeoutMs = 400;

    lastSeen = DIR_UNKNOWN;
    leftCounter = rightCounter = 0;
    lastSeenTime = 0;
    alternateSpinDir = false;

    invertLeft = false;
    invertRight = true;
}

void LineFollower::setSpeeds(int baseSpd, int turnSpd, int spinSpd)
{
    baseSpeed = baseSpd;
    turnSpeed = turnSpd;
    spinSpeed = spinSpd;
}

void LineFollower::setDebounce(int reads)
{
    debounceReads = reads;
}

void LineFollower::setTimeout(unsigned long timeout)
{
    lostTimeoutMs = timeout;
}

void LineFollower::init()
{
    lastSeen = DIR_UNKNOWN;
    leftCounter = rightCounter = 0;
    lastSeenTime = millis();
    alternateSpinDir = false;
}

void LineFollower::update()
{
    bool leftRaw = line_left->isLineDetected();
    bool rightRaw = line_right->isLineDetected();

    // Debounce simple
    leftCounter = leftRaw ? leftCounter + 1 : 0;
    rightCounter = rightRaw ? rightCounter + 1 : 0;

    bool leftDetected = (leftCounter >= debounceReads);
    bool rightDetected = (rightCounter >= debounceReads);

    unsigned long now = millis();

    // === Caso ambos sensores ven línea ===
    if (leftDetected && rightDetected)
    {
        motor_left->setSpeed(baseSpeed);
        motor_right->setSpeed(baseSpeed);
        lastSeen = DIR_UNKNOWN;
        lastSeenTime = now;
        return;
    }

    // === Caso solo el izquierdo ===
    if (leftDetected && !rightDetected)
    {
        motor_left->setSpeed(baseSpeed - turnSpeed);
        motor_right->setSpeed(baseSpeed);
        lastSeen = DIR_LEFT;
        lastSeenTime = now;
        return;
    }

    // === Caso solo el derecho ===
    if (rightDetected && !leftDetected)
    {
        motor_left->setSpeed(baseSpeed);
        motor_right->setSpeed(baseSpeed - turnSpeed);
        lastSeen = DIR_RIGHT;
        lastSeenTime = now;
        return;
    }

    // === Ninguno detecta, pero aún dentro del timeout ===
    if ((now - lastSeenTime) < lostTimeoutMs && lastSeen != DIR_UNKNOWN)
    {
        // Corrección suave: reduce turnSpeed a la mitad
        if (lastSeen == DIR_LEFT)
        {
            motor_left->setSpeed(baseSpeed - turnSpeed / 2);
            motor_right->setSpeed(baseSpeed);
        }
        else if (lastSeen == DIR_RIGHT)
        {
            motor_left->setSpeed(baseSpeed);
            motor_right->setSpeed(baseSpeed - turnSpeed / 2);
        }
        return;
    }

    // === Línea perdida totalmente ===
    // Gira despacio sobre sí mismo para encontrarla
    int spinLeft = alternateSpinDir ? 0 : spinSpeed;
    int spinRight = alternateSpinDir ? spinSpeed : 0;
    motor_left->setSpeed(spinLeft);
    motor_right->setSpeed(spinRight);
    alternateSpinDir = !alternateSpinDir;
}

void LineFollower::setInverted(bool leftInv, bool rightInv)
{
    invertLeft = leftInv;
    invertRight = rightInv;
}
