#include "PistaA.h"
#include "UltrasonicSensor.h"

PistaA::PistaA(Motor *intake, Motor *left, Motor *right, ColorSensor *sensor, int trigD, int echoD, int trigU, int echoU)
{
    intakeMotor = intake;
    motorLeft = left;
    motorRight = right;
    colorSensor = sensor;

    trigDown = trigD;
    echoDown = echoD;
    trigUp = trigU;
    echoUp = echoU;

    pelotaDetectada = false;
    etapa1Completada = false;
    etapa2Completada = false;
    etapa3Completada = false;
    etapa4Completada = false;
    etapa5Completada = false;
}

void PistaA::init()
{
    pinMode(trigDown, OUTPUT);
    pinMode(echoDown, INPUT);
    pinMode(trigUp, OUTPUT);
    pinMode(echoUp, INPUT);
}

void PistaA::runPistaCompleta()
{
    etapa1();
    etapa2();
    etapa3();
    etapa4();
    etapa5();
}

void PistaA::buscarPelota()
{

    float distanciaDown = getUltrasonicDistance(trigDown, echoDown);

    while (distanciaDown > 5.0 || distanciaDown == -1)
    {
        motorLeft->setSpeed(120);
        motorRight->setSpeed(120);
        intakeMotor->setSpeed(150);
        distanciaDown = getUltrasonicDistance(trigDown, echoDown);
        delay(50);
    }

    motorLeft->stop();
    motorRight->stop();
    intakeMotor->stop();
    delay(300);

    int r, g, b;
    colorSensor->readColor(r, g, b);

    if (g < r && r < b)
    {
        // Color amarillo
        intakeMotor->setSpeed(255);
        delay(700);
    }
    else if (b < r && g < r)
    {
        // Color celeste
        intakeMotor->setSpeed(180);
        delay(500);
    }
    else
    {
        // No se reconocio nada
        intakeMotor->setSpeed(-120);
        delay(500);
    }

    intakeMotor->stop();

    while (true)
    {
        float distanciaUp = getUltrasonicDistance(trigUp, echoUp);
        if (distanciaUp > 25.0)
        {
            break;
        }
        delay(200);
    }

    motorLeft->stop();
    motorRight->stop();
    intakeMotor->stop();
}

void PistaA::etapa1()
{
    // if (etapa1Completada)
    // {
    //     intakeMotor->stop();
    //     return;
    // }

    buscarPelota();
    etapa1Completada = true;
}

void PistaA::etapa2()
{
    if (etapa2Completada)
    {
        intakeMotor->stop();
        return;
    }

    if (!etapa1Completada)
        return;

    motorLeft->setSpeed(-80);
    motorRight->setSpeed(-80);
    delay(900);
    motorLeft->stop();
    motorRight->stop();

    motorLeft->setSpeed(-80);
    motorRight->setSpeed(80);
    delay(460);
    motorLeft->stop();
    motorRight->stop();
    delay(500);

    buscarPelota(); // busqueda de pelota

    etapa2Completada = true;
}

void PistaA::etapa3()
{
    if (etapa3Completada)
    {
        intakeMotor->stop();
        return;
    }

    if (!etapa2Completada)
        return;

    motorLeft->setSpeed(-80);
    motorRight->setSpeed(-80);
    delay(900);
    motorLeft->stop();
    motorRight->stop();

    motorLeft->setSpeed(80);
    motorRight->setSpeed(-80);
    delay(460);
    motorLeft->stop();
    motorRight->stop();
    delay(500);

    buscarPelota();

    // Avanzar hasta linea
    /* while (true)
    {
        float distanciaDown = getUltrasonicDistance(trigDown, echoDown);
        if (distanciaDown > 15.0 || distanciaDown == -1)
        {
            break;
        }
        motorLeft->setSpeed(100);
        motorRight->setSpeed(100);
        delay(50);
    }
    motorLeft->stop();
    motorRight->stop(); */

    etapa3Completada = true;
}

void PistaA::etapa4()
{
    if (etapa4Completada)
    {
        intakeMotor->stop();
        return;
    }

    if (!etapa3Completada)
        return;

    motorLeft->setSpeed(-80);
    motorRight->setSpeed(-80);
    delay(900);
    motorLeft->stop();
    motorRight->stop();

    motorLeft->setSpeed(-80);
    motorRight->setSpeed(80);
    delay(460);
    motorLeft->stop();
    motorRight->stop();
    delay(500);

    buscarPelota();

    // Avanzar hasta linea
    /* while (true)
    {
        float distanciaDown = getUltrasonicDistance(trigDown, echoDown);
        if (distanciaDown > 15.0 || distanciaDown == -1)
        {
            break;
        }
        motorLeft->setSpeed(100);
        motorRight->setSpeed(100);
        delay(50);
    }
    motorLeft->stop();
    motorRight->stop(); */

    etapa4Completada = true;
}

void PistaA::etapa5()
{
    if (etapa5Completada)
    {
        intakeMotor->stop();
        return;
    }

    if (!etapa4Completada)
        return;

    motorLeft->setSpeed(-80);
    motorRight->setSpeed(-80);
    delay(900);
    motorLeft->stop();
    motorRight->stop();
    delay(500);

    buscarPelota();

    // Avanzar hasta linea
    /* while (true)
    {
        float distanciaDown = getUltrasonicDistance(trigDown, echoDown);
        if (distanciaDown > 15.0 || distanciaDown == -1)
        {
            break;
        }
        motorLeft->setSpeed(100);
        motorRight->setSpeed(100);
        delay(50);
    }
    motorLeft->stop();
    motorRight->stop(); */

    etapa5Completada = true;
}