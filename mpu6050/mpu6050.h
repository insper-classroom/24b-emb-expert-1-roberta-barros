#ifndef _MPU6050_H
#define _MPU6050_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

// MPU6050 I2C address
#define MPU6050_I2C_ADDR 0x68

// Accelerometer ranges
#define MPU6050_ACC_RANGE_2G  0x00
#define MPU6050_ACC_RANGE_4G  0x08
#define MPU6050_ACC_RANGE_8G  0x10
#define MPU6050_ACC_RANGE_16G 0x18

// Function return values
#define MPU6050_OK     1
#define MPU6050_ERROR  0

// Configuration structure
typedef struct {
    i2c_inst_t *i2c;
    uint sda_pin;
    uint scl_pin;
    uint baudrate;
    uint8_t acc_scale;
} mpu6050_t;

// Function prototypes
void mpu6050_set_config(mpu6050_t *config, i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint8_t acc_scale);
int mpu6050_init(mpu6050_t *config);
int mpu6050_reset(mpu6050_t *config);
int mpu6050_read_acc(mpu6050_t *config, int16_t accel[3]);
int mpu6050_read_gyro(mpu6050_t *config, int16_t gyro[3]);
int mpu6050_read_temp(mpu6050_t *config, int16_t *temp);
int mpu6050_set_motion_detection(mpu6050_t *config, uint8_t threshold, uint8_t duration);

#endif // _MPU6050_H
