#include <stdio.h>
#include "pico/stdlib.h"
#include "mpu6050.h"

int main() {
    stdio_init_all();

    mpu6050_config_t mpu_config;
    mpu6050_set_config(&mpu_config, i2c0, 4, 5, MPU6050_ACCEL_SCALE_2G, MPU6050_GYRO_SCALE_250DEG);

    if (!mpu6050_init(&mpu_config)) {
        printf("Falha em inicializar MPU6050\n");
        return 1;
    }

    int16_t accel[3];
    int16_t gyro[3];
    int16_t temp;

    while (true) {
        if (mpu6050_read_acc(&mpu_config, accel)) {
            printf("Accel X: %f, Y: %f, Z: %f\n", accel[0], accel[1], accel[2]);
        } else {
            printf("Falha no acelerômetro\n");
        }

        if (mpu6050_read_gyro(&mpu_config, gyro)) {
            printf("Gyro X: %f, Y: %f, Z: %f\n", gyro[0], gyro[1], gyro[2]);
        } else {
            printf("Falha no giroscópio\n");
        }

        if (mpu6050_read_temp(&mpu_config, &temp)) {
            float temperature = (temp / 340.0) + 36.53;
            printf("Temperatura: %.2f C\n", temperature);
        } else {
            printf("Falha em ler a temperatura\n");
        }

        sleep_ms(500);
    }

    return 0;
}
