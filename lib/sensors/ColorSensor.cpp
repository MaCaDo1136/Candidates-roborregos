#include "ColorSensor.h"

ColorSensor::ColorSensor(int s0, int s1, int s2, int s3, int outPin)
{
    S0 = s0;
    S1 = s1;
    S2 = s2;
    S3 = s3;
    OUT = outPin;
}

void ColorSensor::init()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(OUT, INPUT);

    // Configurar escala de frecuencia al 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
}

void ColorSensor::readColor(int &r, int &g, int &b)
{
    // --- Filtro rojo ---
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    r = pulseIn(OUT, LOW);

    // --- Filtro verde ---
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    g = pulseIn(OUT, LOW);

    // --- Filtro azul ---
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    b = pulseIn(OUT, LOW);
}
