//
// Created by 27900 on 2023/1/20.
//
#ifndef STM32_REGISTER_GPIO_H
#define STM32_REGISTER_GPIO_H
#pragma once
#include "stm32f401ret6.h"

void GPIO_SetBits(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void GPIO_ReSetBits(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void GPIO_Init(GPIO_TypeDef *GPIOx,GPIO_InitTypeDef *GPIO_InitStruct);
#endif //STM32_REGISTER_GPIO_H
