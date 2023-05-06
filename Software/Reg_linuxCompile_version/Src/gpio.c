//
// Created by LSC on 2023/1/20.
// code with UTF-8
//
#include "gpio.h"
void RCC_GPIO_ENR(GPIO_TypeDef *GPIOx){
    if(GPIOx == GPIOA)      RCC->AHB1ENR |= 1<<0;
    else if(GPIOx == GPIOB) RCC->AHB1ENR |= 1<<1;
    else if(GPIOx == GPIOC) RCC->AHB1ENR |= 1<<2;
    else if(GPIOx == GPIOD) RCC->AHB1ENR |= 1<<3;
    else if(GPIOx == GPIOE) RCC->AHB1ENR |= 1<<4;
    return ;
}
void GPIO_SetBits(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin){
    GPIOx->BSRRL |= GPIO_Pin;
}
void GPIO_ReSetBits(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin){
    GPIOx->BSRRH |= GPIO_Pin;
}
void GPIO_Init(GPIO_TypeDef *GPIOx,GPIO_InitTypeDef *GPIO_InitStruct){
    RCC_GPIO_ENR(GPIOx);
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00;
    for(pinpos = 0x00; pinpos < 16; pinpos++){
        pos = ((uint32_t)0x01) << pinpos;
        currentpin = (GPIO_InitStruct->GPIO_Pin)&pos;
        if(currentpin == pos){
            GPIOx->MODER &= ~(0x3<<(2*pinpos));
            GPIOx->MODER |= ((uint32_t)GPIO_InitStruct->GPIO_Mode)<<(2*pinpos);
            GPIOx->PUPDR &= ~(0x3<<(2*pinpos));
            GPIOx->PUPDR |= ((uint32_t)GPIO_InitStruct->GPIO_Pupd)<<(2*pinpos);
            if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_Out || GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF){
                GPIOx->OSPEEDR &= ~(0x03<<(2*pinpos));
                GPIOx->OSPEEDR |= ((uint32_t)GPIO_InitStruct->GPIO_Speed<<(2*pinpos));
                GPIOx->OTYPER  &= ~(0x01<<pinpos);
                GPIOx->OTYPER  |= ((uint16_t)GPIO_InitStruct->GPIO_OType<<pinpos);
            }
        }
    }
}