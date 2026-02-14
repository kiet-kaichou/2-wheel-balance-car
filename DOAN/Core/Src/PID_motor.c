/*
 * PID_motor.c
 *
 * Created on: Nov 24, 2025
 * Author: User
 */

#include "PID_motor.h"

/* Hàm khởi tạo */
void PID_Motor_Init(MotorPID_t *pid, float kp, float ki, float dt, float pwm_limit)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->dt = dt;
    pid->limit_pwm = pwm_limit;
    pid->limit_i = pwm_limit; // Giới hạn I thường đặt bằng giới hạn PWM

    // Reset các biến nhớ
    pid->prev_encoder = 0;
    pid->I_part = 0.0f;
    pid->v_thuc = 0.0f;
    pid->e_speed = 0.0f;
    pid->v_filter = 0.0f;
}

/* Hàm Reset: Xóa các giá trị tích lũy */
void PID_Motor_Reset(MotorPID_t *pid)
{
    pid->I_part = 0.0f;
    pid->prev_encoder = 0;
}

/* Hàm tính toán PID Tốc độ
 * Thực hiện đúng theo 4 bước trong hình ảnh mô tả
 */
float PID_Motor_Compute(MotorPID_t *pid, float target_speed, long current_encoder)
{
	pid->v_thuc = (float)(current_encoder - pid->prev_encoder) / pid->dt;
    pid->prev_encoder = current_encoder;

    pid->v_filter = 0.85f * pid->v_filter + 0.15f * pid->v_thuc;
    pid->e_speed = target_speed - pid->v_thuc;

    float p_part = pid->Kp * pid->e_speed;

    pid->I_part += (pid->Ki * pid->e_speed * pid->dt);

    if (pid->I_part > pid->limit_i) {
        pid->I_part = pid->limit_i;
    } else if (pid->I_part < -pid->limit_i) {
        pid->I_part = -pid->limit_i;
    }

    float pwm_out = p_part + pid->I_part;

    if (pwm_out > pid->limit_pwm) {
        pwm_out = pid->limit_pwm;
    } else if (pwm_out < -pid->limit_pwm) {
        pwm_out = -pid->limit_pwm;
    }

    return pwm_out;
}

float PID_Motor_Get_VThuc(MotorPID_t *pid)
{
	return pid->v_thuc;
}
