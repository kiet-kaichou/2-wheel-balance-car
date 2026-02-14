#include "stm32f4xx_hal.h"

#ifndef ENCODER_H
#define ENCODER_H

#define ENCODER_1A_PIN      GPIO_PIN_0   // PA0 - EXTI0
#define ENCODER_1A_PORT     GPIOA

#define ENCODER_1B_PIN      GPIO_PIN_0   // PB0
#define ENCODER_1B_PORT     GPIOB

extern volatile int posi;
void encoder1_isr();
void Encoder_GPIO_Init(void);

#endif
