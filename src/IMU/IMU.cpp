#include "IMU/IMU.h"
#include <stdio.h>

IMU::IMU(i2c_inst_t *i2c_port, uint8_t address, uint sda_pin, uint scl_pin)
    : i2c_port_(i2c_port), address_(address), sda_pin_(sda_pin), scl_pin_(scl_pin) {}

void IMU::init()
{
    i2c_init(i2c_port_, 100 * 1000);
    gpio_set_function(sda_pin_, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin_, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin_);
    gpio_pull_up(scl_pin_);
    // Wake up IMU (MPU6050: write 0x00 to PWR_MGMT_1 (0x6B))
    writeRegister(0x6B, 0x00);
}

bool IMU::writeRegister(uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    int res = i2c_write_blocking(i2c_port_, address_, buf, 2, false);
    return res >= 0;
}

bool IMU::readRegisters(uint8_t reg, uint8_t *buf, size_t len)
{
    int res = i2c_write_blocking(i2c_port_, address_, &reg, 1, true);
    if (res < 0)
        return false;
    res = i2c_read_blocking(i2c_port_, address_, buf, len, false);
    return res >= 0;
}

bool IMU::readAccel(int16_t &ax, int16_t &ay, int16_t &az)
{
    uint8_t buf[6];
    if (!readRegisters(0x3B, buf, 6))
        return false;
    ax = (int16_t)((buf[0] << 8) | buf[1]);
    ay = (int16_t)((buf[2] << 8) | buf[3]);
    az = (int16_t)((buf[4] << 8) | buf[5]);
    return true;
}

bool IMU::readGyro(int16_t &gx, int16_t &gy, int16_t &gz)
{
    uint8_t buf[6];
    if (!readRegisters(0x43, buf, 6))
        return false;
    gx = (int16_t)((buf[0] << 8) | buf[1]);
    gy = (int16_t)((buf[2] << 8) | buf[3]);
    gz = (int16_t)((buf[4] << 8) | buf[5]);
    return true;
}

void IMU::printValue()
{
    int16_t ax, ay, az, gx, gy, gz;
    if (readAccel(ax, ay, az))
    {
        printf("IMU Accel: X:%d Y:%d Z:%d ", ax, ay, az);
    }
    else
    {
        printf("IMU Accel: read error ");
    }
    if (readGyro(gx, gy, gz))
    {
        printf("Gyro: X:%d Y:%d Z:%d\n", gx, gy, gz);
    }
    else
    {
        printf("Gyro: read error\n");
    }
}