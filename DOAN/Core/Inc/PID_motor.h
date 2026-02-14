/*
 * PID_motor.h
 *
 * Created on: Nov 24, 2025
 * Author: User
 * Description: Thư viện PID Tốc độ (Speed Loop) cho Robot tự cân bằng
 */

#ifndef INC_PID_MOTOR_H_
#define INC_PID_MOTOR_H_

#include <stdint.h>

typedef struct {
    /* --- CẤU HÌNH (User Settings) --- */
    float Kp;           // Hệ số P (Proportional)
    float Ki;           // Hệ số I (Integral)
    float dt;           // Thời gian lấy mẫu (giây) - Ví dụ: 0.005 (5ms)

    float limit_pwm;    // Giới hạn PWM đầu ra (Ví dụ: 1249)
    float limit_i;      // Giới hạn khâu I (Anti-windup) - Thường bằng limit_pwm

    /* --- BIẾN TRẠNG THÁI (Internal State) --- */
    long prev_encoder;  // Giá trị Encoder lần trước (để tính vận tốc)
    float I_part;       // Giá trị tích lũy khâu I (I_spd_truoc)

    float v_thuc;
    float e_speed;

    float v_filter;

} MotorPID_t;

/* --- CÁC HÀM ĐIỀU KHIỂN --- */

// Hàm khởi tạo các thông số ban đầu
void PID_Motor_Init(MotorPID_t *pid, float kp, float ki, float dt, float pwm_limit);

// Hàm tính toán chính (Gọi trong ngắt Timer hoặc vòng lặp định thời)
// Input: Tốc độ mong muốn (Target) và Encoder hiện tại
// Output: Giá trị PWM cần xuất ra động cơ
float PID_Motor_Compute(MotorPID_t *pid, float target_speed, long current_encoder);

// Hàm reset bộ PID (Dùng khi robot bị ngã hoặc khởi động lại)
void PID_Motor_Reset(MotorPID_t *pid);

float PID_Motor_Get_VThuc(MotorPID_t *pid);

#endif /* INC_PID_MOTOR_H_ */
