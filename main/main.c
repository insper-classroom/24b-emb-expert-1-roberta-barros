/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "mpu6050.h"

int main() {
    stdio_init_all();

    mpu6050_t mpu;
    mpu6050_set_config(&mpu, i2c0, 4, 5, MPU6050_ACC_RANGE_2G);

    if (mpu6050_init(&mpu) == MPU6050_OK) {
        printf("MPU6050 initialized successfully.\n");
    } else {
        printf("Failed to initialize MPU6050.\n");
        return 1;
    }

    int16_t accel[3], gyro[3];
    int16_t temp;

    while (1) {
        if (mpu6050_read_acc(&mpu, accel) == MPU6050_OK) {
            printf("Accel X: %d, Y: %d, Z: %d\n", accel[0], accel[1], accel[2]);
        }

        if (mpu6050_read_gyro(&mpu, gyro) == MPU6050_OK) {
            printf("Gyro X: %d, Y: %d, Z: %d\n", gyro[0], gyro[1], gyro[2]);
        }

        if (mpu6050_read_temp(&mpu, &temp) == MPU6050_OK) {
            printf("Temperature: %.2f C\n", (temp / 340.0) + 36.53);
        }

        sleep_ms(1000);
    }

    return 0;
}
