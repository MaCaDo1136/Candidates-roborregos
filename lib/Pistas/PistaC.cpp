#include "PistaC.h"
#include "UltrasonicSensor.h"

PistaC::PistaC(Motor *intake, Motor *left, Motor *right, ColorSensor *sensor, int trigD, int echoD, int trigU, int echoU)
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

void PistaC::init()
{
    pinMode(trigDown, OUTPUT);
    pinMode(echoDown, INPUT);
    pinMode(trigUp, OUTPUT);
    pinMode(echoUp, INPUT);
}

void PistaC::runPistaCompleta()
{
    etapa1();
    /*
    etapa2();
    etapa3();
    etapa4();
    etapa5();
    */
}

void PistaC::girar90GradosLeft()
{
    motorLeft->setSpeed(-80);
    motorRight->setSpeed(80);
    delay(500);
    motorLeft->stop();
    motorRight->stop();
    delay(1500);
}

void PistaC::girar90GradosRight()
{
    motorLeft->setSpeed(80);
    motorRight->setSpeed(-80);
    delay(500);
    motorLeft->stop();
    motorRight->stop();
    delay(1500);
}

void PistaC::buscarPelota()
{
    //
}

void PistaC::adelante()
{
    motorLeft->setSpeed(-70);
    motorRight->setSpeed(-92);
    delay(1000);
    motorLeft->stop();
    motorRight->stop();
    delay(1500);
}

void PistaC::etapa1()
{
    if (etapa1Completada)
    {
        return;
    }

    // buscarPelota();

    /*motorLeft->setSpeed(-180);
    motorRight->setSpeed(-180);
    delay(900);
    motorLeft->stop();
    motorRight->stop();
    delay(15000);
*/
    adelante();
    girar90GradosRight();
    adelante();
    girar90GradosLeft();
    adelante();
    adelante();
    girar90GradosLeft();
    adelante();
    adelante();
    girar90GradosLeft();
    adelante();
    girar90GradosRight();
    adelante();
    girar90GradosRight();
    adelante();
    adelante();
    etapa1Completada = true;
}

void PistaC::etapa2()
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

void PistaC::etapa3()
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

void PistaC::etapa4()
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

void PistaC::etapa5()
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