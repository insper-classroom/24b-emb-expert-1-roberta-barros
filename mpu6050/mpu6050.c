#include "mpu6050.h"

void mpu6050_set_config(mpu6050_config_t *config, i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint8_t accel_scale, uint8_t gyro_scale) {
    config->i2c = i2c;
    config->sda_pin = sda_pin;
    config->scl_pin = scl_pin;
    config->accel_scale = accel_scale;
    config->gyro_scale = gyro_scale;
}

int mpu6050_init(const mpu6050_config_t *config) {
    i2c_init(config->i2c, 400 * 1000);
    gpio_set_function(config->sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(config->scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(config->sda_pin);
    gpio_pull_up(config->scl_pin);

    uint8_t who_am_i = 0;
    uint8_t reg = MPU6050_REG_WHO_AM_I;
    int ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, &reg, 1, true);
    if (ret != PICO_ERROR_GENERIC) {
        ret = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDRESS, &who_am_i, 1, false);
        if (who_am_i != MPU6050_I2C_ADDRESS) {
            return 0; 
        }
    } else {
        return 0;
    }

    uint8_t buffer[2];

    buffer[0] = MPU6050_REG_PWR_MGMT_1;
    buffer[1] = 0x00;
    ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    buffer[0] = MPU6050_REG_ACCEL_CONFIG;
    buffer[1] = config->accel_scale << 3;
    ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    buffer[0] = MPU6050_REG_GYRO_CONFIG;
    buffer[1] = config->gyro_scale << 3;
    ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    return 1; 
}

int mpu6050_reset(const mpu6050_config_t *config) {
    uint8_t buffer[2];

    buffer[0] = MPU6050_REG_PWR_MGMT_1;
    buffer[1] = 0x80; 
    int ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    sleep_ms(100);

    return 1; 
}

int mpu6050_read_acc(const mpu6050_config_t *config, int16_t accel[3]) {
    uint8_t buffer[6];
    uint8_t reg = MPU6050_REG_ACCEL_XOUT_H;

    int ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, &reg, 1, true);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    ret = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 6, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    accel[0] = (int16_t)(buffer[0] << 8 | buffer[1]);
    accel[1] = (int16_t)(buffer[2] << 8 | buffer[3]);
    accel[2] = (int16_t)(buffer[4] << 8 | buffer[5]);

    return 1; 
}

int mpu6050_read_gyro(const mpu6050_config_t *config, int16_t gyro[3]) {
    uint8_t buffer[6];
    uint8_t reg = MPU6050_REG_GYRO_XOUT_H;

    int ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, &reg, 1, true);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    ret = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 6, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0;
    }

    gyro[0] = (int16_t)(buffer[0] << 8 | buffer[1]);
    gyro[1] = (int16_t)(buffer[2] << 8 | buffer[3]);
    gyro[2] = (int16_t)(buffer[4] << 8 | buffer[5]);

    return 1; 
}

int mpu6050_read_temp(const mpu6050_config_t *config, int16_t *temp) {
    uint8_t buffer[2];
    uint8_t reg = MPU6050_REG_TEMP_OUT_H;

    int ret = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDRESS, &reg, 1, true);
    if (ret == PICO_ERROR_GENERIC) {
        return 0; 
    }

    ret = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDRESS, buffer, 2, false);
    if (ret == PICO_ERROR_GENERIC) {
        return 0;
    }

    *temp = (int16_t)(buffer[0] << 8 | buffer[1]);

    return 1;
}
