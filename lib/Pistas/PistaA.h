#pragma once
#include <Arduino.h>
#include "UltrasonicSensor.h"
#include "ColorSensor.h"
#include "Motors.h"

class PistaA
{
private:
    Motor *intakeMotor;
    Motor *motorLeft;
    Motor *motorRight;
    ColorSensor *colorSensor;
    int trigDown, echoDown;
    int trigUp, echoUp;

    bool pelotaDetectada;
    bool etapa1Completada;
    bool etapa2Completada;
    bool etapa3Completada;
    bool etapa4Completada;
    bool etapa5Completada;

public:
    PistaA(Motor *intake, Motor *left, Motor *right, ColorSensor *sensor, int trigD, int echoD, int trigU, int echoU);
    void init();
    void buscarPelota();
    void etapa1();
    void etapa2();
    void etapa3();
    void etapa4();
    void etapa5();
    void runPistaCompleta();
};
