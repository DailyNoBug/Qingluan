//
// Created by 27900 on 2023/1/27.
//

#ifndef STM32_REGISTER_PWM_H
#define STM32_REGISTER_PWM_H
#pragma once
#include "stm32f401ret6.h"
#include "gpio.h"
void TIM_Init(uint16_t arr,uint16_t psc);
void TIM_UnLock();
void PWM_All(uint16_t x);
#endif //STM32_REGISTER_PWM_H
