//
// Created by 27900 on 2023/2/1.
//

#ifndef STM32_REGISTER_MPU6050_H
#define STM32_REGISTER_MPU6050_H
#pragma once
#include "stm32f401ret6.h"
#include "iic.h"
#define MPU6050_ADDR 0xD0
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75
void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data);
void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void MPU6050_Init();
void MPU6050_Read_Accel (void);
void MPU6050_Read_Gyro (void);
void MPU6050_Read_temp(void);
#endif //STM32_REGISTER_MPU6050_H
