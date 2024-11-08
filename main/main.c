#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "mpu6050.h"

#define ACCEL_SCALE_FACTOR 16384.0 // ajuste conforme o valor de escala de acelerômetro
#define GYRO_SCALE_FACTOR 131.0    // ajuste conforme o valor de escala de giroscópio

int main() {
    stdio_init_all();
    printf("Início\n");

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
            // Converte os valores do acelerômetro para float
            float accel_x = accel[0] / ACCEL_SCALE_FACTOR;
            float accel_y = accel[1] / ACCEL_SCALE_FACTOR;
            float accel_z = accel[2] / ACCEL_SCALE_FACTOR;
            printf("Accel X: %.2f, Y: %.2f, Z: %.2f\n", accel_x, accel_y, accel_z);
        } else {
            printf("Falha no acelerômetro\n");
        }

        if (mpu6050_read_gyro(&mpu_config, gyro)) {
            // Converte os valores do giroscópio para float
            float gyro_x = gyro[0] / GYRO_SCALE_FACTOR;
            float gyro_y = gyro[1] / GYRO_SCALE_FACTOR;
            float gyro_z = gyro[2] / GYRO_SCALE_FACTOR;
            printf("Gyro X: %.2f, Y: %.2f, Z: %.2f\n", gyro_x, gyro_y, gyro_z);
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
