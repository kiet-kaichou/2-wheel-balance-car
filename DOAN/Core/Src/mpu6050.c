#include "mpu6050.h"
#include "main.h"
#include <math.h>

extern I2C_HandleTypeDef hi2c1;

#define RAD_TO_DEG 57.2957795f
#define PITCH_OFFSET 5.0f

float pre_pitch = 0.0f;

void mpu6050_init()
{
  uint8_t set_gyro_range = GYRO_RANGE_500;
  uint8_t set_accel_range = ACCEL_RANGE_4G;
  uint8_t set_sleep_mode = SLEEP_MODE_OFF;

  if (HAL_I2C_IsDeviceReady(&hi2c1, SENSOR_ADDR, 1, 2) != HAL_OK) {
      return;
  }

  HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDR, GYRO_CONFIG_REG, 1, &set_gyro_range, 1, 2);
  HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDR, ACCEL_CONFIG_REG, 1, &set_accel_range, 1, 2);
  HAL_I2C_Mem_Write(&hi2c1, SENSOR_ADDR, SLEEP_MODE_REG, 1, &set_sleep_mode, 1, 2);
}

void mpu6050_read_all(accel_data *accel, gyro_data *gyro)
{
    uint8_t buffer[14];
    if (HAL_I2C_Mem_Read(&hi2c1, SENSOR_ADDR, 0x3B, 1, buffer, 14, 2) == HAL_OK)
    {
        // Accel
        accel->x_val_raw = (int16_t)(buffer[0] << 8 | buffer[1]);
        accel->y_val_raw = (int16_t)(buffer[2] << 8 | buffer[3]);
        accel->z_val_raw = (int16_t)(buffer[4] << 8 | buffer[5]);

        accel->x_val = (float)accel->x_val_raw / ACCEL_SENSITIVITY;
        accel->y_val = (float)accel->y_val_raw / ACCEL_SENSITIVITY;
        accel->z_val = (float)accel->z_val_raw / ACCEL_SENSITIVITY;

        // Gyro (Bỏ qua Temp tại buffer[6], [7])
        gyro->x_val_raw = (int16_t)(buffer[8] << 8 | buffer[9]);
        gyro->y_val_raw = (int16_t)(buffer[10] << 8 | buffer[11]);
        gyro->z_val_raw = (int16_t)(buffer[12] << 8 | buffer[13]);

        gyro->x_val = (float)gyro->x_val_raw / GYRO_SENSITIVITY;
        gyro->y_val = (float)gyro->y_val_raw / GYRO_SENSITIVITY;
        gyro->z_val = (float)gyro->z_val_raw / GYRO_SENSITIVITY;

        accel->pitch_angle = (-atan2f(accel->x_val, sqrtf(accel->y_val * accel->y_val + accel->z_val * accel->z_val)) * RAD_TO_DEG) + PITCH_OFFSET;
    }
}

void mpu6050_complementary_filter(gyro_data *gyro, accel_data *accel, filtered_angle_t *angle, float dt)
{
    const float alpha = 0.98f;
    angle->pitch = alpha * (pre_pitch + gyro->y_val * dt) + (1.0f - alpha) * accel->pitch_angle;

    pre_pitch = angle->pitch;
}
