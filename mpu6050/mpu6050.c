#include "mpu6050.h"

void mpu6050_set_config(mpu6050_t *config, i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint8_t acc_scale) {
    config->i2c = i2c;
    config->sda_pin = sda_pin;
    config->scl_pin = scl_pin;
    config->baudrate = 100000; // Standard I2C baudrate
    config->acc_scale = acc_scale;
}

int mpu6050_init(mpu6050_t *config) {
    // Initialize I2C
    i2c_init(config->i2c, config->baudrate);
    gpio_set_function(config->sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(config->scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(config->sda_pin);
    gpio_pull_up(config->scl_pin);

    // Wake up the MPU6050 as it starts in sleep mode
    uint8_t buf[2];
    buf[0] = 0x6B; // PWR_MGMT_1 register
    buf[1] = 0x00; // Set to zero (wakes up the MPU6050)
    int result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, buf, 2, false);

    // Set accelerometer range
    buf[0] = 0x1C; // ACCEL_CONFIG register
    buf[1] = config->acc_scale;
    result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, buf, 2, false);

    return result == PICO_ERROR_GENERIC ? MPU6050_ERROR : MPU6050_OK;
}

int mpu6050_reset(mpu6050_t *config) {
    uint8_t buf[2];
    buf[0] = 0x6B; // PWR_MGMT_1 register
    buf[1] = 0x80; // Reset device
    int result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, buf, 2, false);
    sleep_ms(100); // Wait for the reset to complete

    return result == PICO_ERROR_GENERIC ? MPU6050_ERROR : MPU6050_OK;
}

int mpu6050_read_acc(mpu6050_t *config, int16_t accel[3]) {
    uint8_t reg = 0x3B; // ACCEL_XOUT_H register
    uint8_t buffer[6];

    int result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, &reg, 1, true);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    result = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDR, buffer, 6, false);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    accel[0] = (buffer[0] << 8) | buffer[1];
    accel[1] = (buffer[2] << 8) | buffer[3];
    accel[2] = (buffer[4] << 8) | buffer[5];

    return MPU6050_OK;
}

int mpu6050_read_gyro(mpu6050_t *config, int16_t gyro[3]) {
    uint8_t reg = 0x43; // GYRO_XOUT_H register
    uint8_t buffer[6];

    int result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, &reg, 1, true);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    result = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDR, buffer, 6, false);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    gyro[0] = (buffer[0] << 8) | buffer[1];
    gyro[1] = (buffer[2] << 8) | buffer[3];
    gyro[2] = (buffer[4] << 8) | buffer[5];

    return MPU6050_OK;
}

int mpu6050_read_temp(mpu6050_t *config, int16_t *temp) {
    uint8_t reg = 0x41; // TEMP_OUT_H register
    uint8_t buffer[2];

    int result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, &reg, 1, true);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    result = i2c_read_blocking(config->i2c, MPU6050_I2C_ADDR, buffer, 2, false);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    *temp = (buffer[0] << 8) | buffer[1];

    return MPU6050_OK;
}

int mpu6050_set_motion_detection(mpu6050_t *config, uint8_t threshold, uint8_t duration) {
    uint8_t buf[2];

    // Set motion threshold
    buf[0] = 0x1F; // MOT_THR register
    buf[1] = threshold;
    int result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, buf, 2, false);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    // Set motion duration
    buf[0] = 0x20; // MOT_DUR register
    buf[1] = duration;
    result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, buf, 2, false);
    if (result == PICO_ERROR_GENERIC) return MPU6050_ERROR;

    // Enable motion detection interrupt
    buf[0] = 0x38; // INT_ENABLE register
    buf[1] = 0x40; // Enable motion interrupt
    result = i2c_write_blocking(config->i2c, MPU6050_I2C_ADDR, buf, 2, false);

    return result == PICO_ERROR_GENERIC ? MPU6050_ERROR : MPU6050_OK;
}
