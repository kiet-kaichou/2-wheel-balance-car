/*
 * state_machine.h
 * Adjusted for STM32F407 Project
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "main.h"

// Định nghĩa các trạng thái của Robot
typedef enum {
    ROBOT_IDLE,     // Chờ (chưa chạy)
    ROBOT_START,    // Bắt đầu
    ROBOT_FORWARD,  // Đang nghiêng về trước -> Chạy tới
    ROBOT_BACKWARD, // Đang nghiêng về sau -> Chạy lùi
    ROBOT_STOP,     // Dừng khẩn cấp
    ROBOT_BALANCED  // Đã cân bằng (Optional)
} Robot_State;

// Khai báo hàm xử lý trạng thái
// Lưu ý: Input là góc 'pitch' đã lọc (float) thay vì struct accel thô
Robot_State robot_idle_state(TIM_HandleTypeDef * timer);
Robot_State robot_start_state(float pitch);
Robot_State robot_forward_state(float pitch);
Robot_State robot_backward_state(float pitch);
Robot_State robot_stopped_state(TIM_HandleTypeDef * timer);

// Hàm hỗ trợ điều khiển chiều động cơ (Helper)
void Set_Motor_Direction(int direction); // 1: Forward, -1: Backward

#endif /* INC_STATE_MACHINE_H_ */
