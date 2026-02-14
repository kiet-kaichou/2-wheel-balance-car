/*
 * PID_motor.h
 *
 * Created on: Nov 22, 2025
 * Author: Gemini AI
 */

#ifndef INC_PID_MOTOR_H_
#define INC_PID_MOTOR_H_

/* Cấu trúc PID điều khiển tốc độ Motor */
typedef struct
{
    // Hệ số điều khiển
    float Kp;           // Hệ số P
    float Ki;           // Hệ số I
    float dt;           // Chu kỳ lấy mẫu PID (ví dụ 0.01s)

    // Giới hạn
    float limit_pwm;    // PWM tối đa cho phép (ví dụ 1000)
    float limit_i;      // Giới hạn khâu I (anti-windup)

    // Trạng thái PID
    long  prev_encoder; // Giá trị encoder lần trước
    float I_part;       // Thành phần tích phân
    float out_pwm;      // Giá trị PWM cuối cùng
} MotorPID_t;

/* Hàm khởi tạo PID Motor */
void PID_Motor_Init(MotorPID_t *pid, float kp, float ki, float dt, float pwm_limit);

/* Hàm tính toán PID tốc độ */
float PID_Motor_Compute(MotorPID_t *pid, float target_speed, long current_encoder);

#endif /* INC_PID_MOTOR_H_ */
