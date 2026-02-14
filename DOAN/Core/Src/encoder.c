#include "encoder.h"

volatile int posi = 0; // Position updated by the encoder

void Encoder_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // PA0 = A = EXTI0
    GPIO_InitStruct.Pin = ENCODER_1A_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER_1A_PORT, &GPIO_InitStruct);

    // PB0 = B input
    GPIO_InitStruct.Pin = ENCODER_1B_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER_1B_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void encoder1_isr()
{
	int B = HAL_GPIO_ReadPin(ENCODER_1B_PORT, ENCODER_1B_PIN);

    if (B == GPIO_PIN_SET) {
        posi++;
    } else {
        posi--;
    }
}
