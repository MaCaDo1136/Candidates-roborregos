#pragma once
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "Sensors/SensorBase.h"

class IMU : public SensorBase
{
public:
    IMU(i2c_inst_t *i2c_port, uint8_t address, uint sda_pin, uint scl_pin);
    void init();
    bool readAccel(int16_t &ax, int16_t &ay, int16_t &az);
    bool readGyro(int16_t &gx, int16_t &gy, int16_t &gz);
    void printValue() override;

private:
    i2c_inst_t *i2c_port_;
    uint8_t address_;
    uint sda_pin_, scl_pin_;
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegisters(uint8_t reg, uint8_t *buf, size_t len);
};