/*
 * pid_controller.h
 *
 * Updated for Self-Balancing Robot
 */

#ifndef INC_PID_H_
#define INC_PID_H_

/* Struct containing variables for the PID controller */
typedef struct
{
	// --- Cấu hình (User settings) ---
	float sampling_time; // Thời gian lấy mẫu (ms) - Ví dụ: 1.25ms

	// Hệ số PID
	float kp;
	float ki;
	float kd;

	// Giới hạn đầu ra (Output limits)
	float lim_max;       // Ví dụ: 1000 (PWM max)
	float lim_min;       // Ví dụ: -1000 (PWM min cho chiều ngược lại)

	// Giới hạn khâu I (Anti-windup limits)
	float lim_max_int;   // Giới hạn tích phân (ví dụ: 500)
	float lim_min_int;   // Ví dụ: -500

	// --- Biến nội bộ (Internal memory) ---
	float proportional;
	float integral;      // Tích lũy lỗi
	float derivative;
	float prev_error;    // Lỗi lần trước
	float out;           // Kết quả đầu ra
} PIDController;

/* Controller functions */
void PIDController_Init(PIDController * controller);
float PIDController_Update(PIDController * controller, float setpoint, float measurement);
void PIDController_Reset(PIDController * controller); // Hàm reset khi robot ngã

#endif /* INC_PID_H_ */
