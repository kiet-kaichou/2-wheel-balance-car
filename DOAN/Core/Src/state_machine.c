/*
 * state_machine.c
 * Adjusted for STM32F407 Project
 */

#include "state_machine.h"
#include <math.h>

/* Helper: Hàm cài đặt chiều quay động cơ an toàn
 * Dir = 1: Forward (Chạy tới)
 * Dir = -1: Backward (Chạy lùi)
 */
void Set_Motor_Direction(int direction)
{
    if (direction == 1) // FORWARD
    {
        // Motor 1 (Left) - Điều chỉnh High/Low tùy cách đấu dây của bạn
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

        // Motor 2 (Right)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    }
    else if (direction == -1) // BACKWARD
    {
        // Motor 1 (Left)
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

        // Motor 2 (Right)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    }
}

/* IDLE: Tắt động cơ */
Robot_State robot_idle_state(TIM_HandleTypeDef * timer)
{
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_3, 0);
    return ROBOT_IDLE;
}

/* START: Quyết định hướng đi dựa trên góc nghiêng ban đầu */
Robot_State robot_start_state(float pitch)
{
    if (pitch < -0.5f) // Nghiêng ra sau -> Cần lùi
    {
        return ROBOT_BACKWARD;
    }
    else if (pitch > 0.5f) // Nghiêng ra trước -> Cần tới
    {
        return ROBOT_FORWARD;
    }
    else
    {
        return ROBOT_START; // Đứng yên
    }
}

/* FORWARD: Robot đang nghiêng tới, cần chạy tới để cân bằng */
Robot_State robot_forward_state(float pitch)
{
    // Cài đặt cứng chiều quay là TỚI
    Set_Motor_Direction(1);

    // Nếu góc chuyển sang âm (ngã ra sau) -> Chuyển trạng thái
    if (pitch < 0.0f)
    {
        return ROBOT_BACKWARD;
    }

    return ROBOT_FORWARD;
}

/* BACKWARD: Robot đang nghiêng lui, cần chạy lùi */
Robot_State robot_backward_state(float pitch)
{
    // Cài đặt cứng chiều quay là LÙI
    Set_Motor_Direction(-1);

    // Nếu góc chuyển sang dương (ngã ra trước) -> Chuyển trạng thái
    if (pitch > 0.0f)
    {
        return ROBOT_FORWARD;
    }

    return ROBOT_BACKWARD;
}

/* STOP: Dừng động cơ */
Robot_State robot_stopped_state(TIM_HandleTypeDef * timer)
{
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_3, 0);
    return ROBOT_STOP;
}
