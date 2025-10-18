#include "LineFollower.h"

LineFollower::LineFollower(LineSensor *left, LineSensor *right, Motor *mLeft, Motor *mRight)
{
    line_left = left;
    line_right = right;
    motor_left = mLeft;
    motor_right = mRight;

    baseSpeed = 80;
    turnSpeed = 70;
    spinSpeed = 60;
    debounceReads = 1;
    lostTimeoutMs = 400;

    lastSeen = DIR_UNKNOWN;
    leftCounter = rightCounter = 0;
    lastSeenTime = 0;
    alternateSpinDir = false;

    invertLeft = false;
    invertRight = false;
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

    // Helper para aplicar inversión
    auto applyInvert = [&](Motor *motor, int speed, bool inverted)
    {
        motor->setSpeed(inverted ? -speed : speed);
    };

    // === Caso ambos sensores ven línea ===
    if (leftDetected && rightDetected)
    {
        applyInvert(motor_left, baseSpeed, invertLeft);
        applyInvert(motor_right, baseSpeed, invertRight);
        lastSeen = DIR_UNKNOWN;
        lastSeenTime = now;
        return;
    }

    // === Caso solo el izquierdo ===
    if (leftDetected && !rightDetected)
    {
        applyInvert(motor_left, baseSpeed - turnSpeed, invertLeft);
        applyInvert(motor_right, baseSpeed, invertRight);
        lastSeen = DIR_LEFT;
        lastSeenTime = now;
        return;
    }

    // === Caso solo el derecho ===
    if (rightDetected && !leftDetected)
    {
        applyInvert(motor_left, baseSpeed, invertLeft);
        applyInvert(motor_right, baseSpeed - turnSpeed, invertRight);
        lastSeen = DIR_RIGHT;
        lastSeenTime = now;
        return;
    }

    // === Ninguno detecta, pero aún dentro del timeout ===
    if ((now - lastSeenTime) < lostTimeoutMs && lastSeen != DIR_UNKNOWN)
    {
        if (lastSeen == DIR_LEFT)
        {
            applyInvert(motor_left, baseSpeed - turnSpeed / 1.5, invertLeft);
            applyInvert(motor_right, baseSpeed, invertRight);
        }
        else if (lastSeen == DIR_RIGHT)
        {
            applyInvert(motor_left, baseSpeed, invertLeft);
            applyInvert(motor_right, baseSpeed - turnSpeed / 1.5, invertRight);
        }
        return;
    }

    // === Línea perdida totalmente ===
    int spinLeft = alternateSpinDir ? -spinSpeed : spinSpeed;
    int spinRight = alternateSpinDir ? spinSpeed : -spinSpeed;
    applyInvert(motor_left, spinLeft, invertLeft);
    applyInvert(motor_right, spinRight, invertRight);
    alternateSpinDir = !alternateSpinDir;
}

void LineFollower::setInverted(bool leftInv, bool rightInv)
{
    invertLeft = leftInv;
    invertRight = rightInv;
}
