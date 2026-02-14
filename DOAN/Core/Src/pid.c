/*
 * pid_controller.c
 *
 * Updated for Self-Balancing Robot
 */

#include "pid.h"
#include <math.h>

/* Khởi tạo các giá trị ban đầu */
void PIDController_Init(PIDController * controller)
{
	controller->integral = 0.0f;
	controller->prev_error = 0.0f;
	controller->derivative = 0.0f;
	controller->out = 0.0f;

	// Đặt mặc định nếu người dùng quên set (An toàn)
	if (controller->sampling_time == 0) controller->sampling_time = 1.25f; // 1.25ms mặc định
}

/* Hàm Reset: Gọi hàm này khi robot bị ngã hoặc trước khi bắt đầu chạy lại
   để xóa các tích lũy lỗi cũ */
void PIDController_Reset(PIDController * controller)
{
    controller->integral = 0.0f;
    controller->prev_error = 0.0f;
    controller->out = 0.0f;
}

/* Tính toán PID
 * setpoint: Giá trị mong muốn (Ví dụ: Góc 0 độ)
 * measurement: Giá trị thực tế (Ví dụ: Góc đo được từ MPU6050)
 */
float PIDController_Update(PIDController *controller, float setpoint, float measurement)
{
	float dt = controller->sampling_time / 1000.0f; // Chuyển ms sang giây (seconds)

	// 1. Tính lỗi (QUAN TRỌNG: Không dùng fabs ở đây để giữ chiều âm/dương)
	float error = setpoint - measurement;

	// 2. Khâu P (Proportional)
	controller->proportional = controller->kp * error;

	// 3. Khâu I (Integral)
	// Tích phân theo thời gian thực: I = I + (Ki * error * dt)
	controller->integral += (controller->ki * error * dt);

	// --> Anti-windup (Bão hòa khâu I): Ngăn I tăng quá lớn gây vọt lố
	if (controller->integral > controller->lim_max_int) {
	    controller->integral = controller->lim_max_int;
	} else if (controller->integral < controller->lim_min_int) {
	    controller->integral = controller->lim_min_int;
	}

	// 4. Khâu D (Derivative)
	// D = Kd * (Delta Error / Delta t)
	controller->derivative = controller->kd * (error - controller->prev_error) / dt;

	// 5. Tổng hợp PID
	controller->out = controller->proportional + controller->integral + controller->derivative;

	// 6. Bão hòa đầu ra (Output Clamping)
	// Giới hạn PWM hoặc tốc độ mục tiêu trong khoảng cho phép


    if (controller->out > controller->lim_max) {
        controller->out = controller->lim_max;
    } else if (controller->out < controller->lim_min) {
        controller->out = controller->lim_min;
    }


	// 7. Lưu lỗi hiện tại cho vòng lặp sau
	controller->prev_error = error;

	return controller->out;
}
