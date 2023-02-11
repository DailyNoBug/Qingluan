//
// Created by 27900 on 2023/2/1.
//

#ifndef STM32_REGISTER_IIC_H
#define STM32_REGISTER_IIC_H
#pragma once
#include "stm32f401ret6.h"
#include "gpio.h"
#define MPU_I2C I2C2
void I2C_Init(I2C_TypeDef *I2cx);
void I2C_Start(I2C_TypeDef *I2cx);
void I2C_Write(I2C_TypeDef *I2cx,uint8_t data);
void I2C_Address(I2C_TypeDef *I2cx,uint8_t Address);
void I2C_WriteMulti(I2C_TypeDef *I2cx,uint8_t *data,uint8_t size);
void I2C_Read(I2C_TypeDef *I2cx,uint8_t Address,uint8_t *buffer,uint8_t size);
void I2C_Stop(I2C_TypeDef *I2cx);
#endif //STM32_REGISTER_IIC_H
