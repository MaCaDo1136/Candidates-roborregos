#pragma once
#include <Arduino.h>

class ColorSensor
{
private:
    int S0, S1, S2, S3, OUT;

public:
    ColorSensor(int s0, int s1, int s2, int s3, int outPin);
    void init();
    void readColor(int &r, int &g, int &b);
};
