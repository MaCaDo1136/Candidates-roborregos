#pragma once
#include <stdio.h>

class SensorBase
{
public:
    virtual ~SensorBase() {}
    virtual void printValue() = 0; // Pure virtual
};