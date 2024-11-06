#ifndef MPU6050_H
#define MPU6050_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define MPU6050_I2C_ADDRESS 0x68

#define MPU6050_REG_SMPLRT_DIV     0x19
#define MPU6050_REG_CONFIG         0x1A
#define MPU6050_REG_GYRO_CONFIG    0x1B
#define MPU6050_REG_ACCEL_CONFIG   0x1C
#define MPU6050_REG_INT_ENABLE     0x38
#define MPU6050_REG_ACCEL_XOUT_H   0x3B
#define MPU6050_REG_ACCEL_XOUT_L   0x3C
#define MPU6050_REG_ACCEL_YOUT_H   0x3D
#define MPU6050_REG_ACCEL_YOUT_L   0x3E
#define MPU6050_REG_ACCEL_ZOUT_H   0x3F
#define MPU6050_REG_ACCEL_ZOUT_L   0x40
#define MPU6050_REG_TEMP_OUT_H     0x41
#define MPU6050_REG_TEMP_OUT_L     0x42
#define MPU6050_REG_GYRO_XOUT_H    0x43
#define MPU6050_REG_GYRO_XOUT_L    0x44
#define MPU6050_REG_GYRO_YOUT_H    0x45
#define MPU6050_REG_GYRO_YOUT_L    0x46
#define MPU6050_REG_GYRO_ZOUT_H    0x47
#define MPU6050_REG_GYRO_ZOUT_L    0x48
#define MPU6050_REG_PWR_MGMT_1     0x6B
#define MPU6050_REG_WHO_AM_I       0x75

#define MPU6050_ACCEL_SCALE_2G     0
#define MPU6050_ACCEL_SCALE_4G     1
#define MPU6050_ACCEL_SCALE_8G     2
#define MPU6050_ACCEL_SCALE_16G    3
#define MPU6050_GYRO_SCALE_250DEG  0
#define MPU6050_GYRO_SCALE_500DEG  1
#define MPU6050_GYRO_SCALE_1000DEG 2
#define MPU6050_GYRO_SCALE_2000DEG 3

typedef struct {
    i2c_inst_t *i2c;
    uint sda_pin;
    uint scl_pin;

    uint8_t accel_scale;

    uint8_t gyro_scale;
} mpu6050_config_t;

void mpu6050_set_config(mpu6050_config_t *config, i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint8_t accel_scale, uint8_t gyro_scale);
int mpu6050_init(const mpu6050_config_t *config);
int mpu6050_reset(const mpu6050_config_t *config);
int mpu6050_read_acc(const mpu6050_config_t *config, int16_t accel[3]);
int mpu6050_read_gyro(const mpu6050_config_t *config, int16_t gyro[3]);
int mpu6050_read_temp(const mpu6050_config_t *config, int16_t *temp);

#endif 
