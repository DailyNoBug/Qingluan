//
// Created by 27900 on 2023/1/27.
//
#include "pwm.h"
/**
 * 本函数暂时不具有普适性，仅仅适用于初始化TIM3，之后将会进行普适性适配
 * 使用TIM3来产生PWM波,AF复用为02
 */
void TIM_Init(uint16_t arr,uint16_t psc){
    RCC->APB1ENR |= 1<<1;       //使能TIM3
    RCC->AHB1ENR |= 1<<0 | 1<<1;//使能GPIOA和GPIOB
    GPIOA->MODER |= (0x2<<12) | (0x2<<14);
    GPIOB->MODER |= (0x2<<0) | (0x2<<2);
    GPIOA->AFRL  |= (0x2<<24) | (0x2<<28);
    GPIOB->AFRL  |= (0x2<<0) | (0x2<<4);
    //PA6 -> CH1 PA7 -> CH2 PB0 -> CH3 PB1 -> CH4;
    TIM3->CCMR1 |= (0x1<<3) | (0x6<<4);
    TIM3->CCMR1 |= (0x1<<11) | (0x6<<12);
    TIM3->CCMR2 |= (0x6<<4) | (0x6<<12);
    TIM3->CCMR2 |= (0x1<<3) | (0x1<<11);
    TIM3->CCER |= (1<<0) | (1<<4) | (1<<8) | (1<<12);
    TIM3->PSC = psc;
    TIM3->ARR = arr;
    TIM3->EGR |= 1<<0;
    TIM3->CCR1 = 2000;
    TIM3->CCR2 = 2000;
    TIM3->CCR3 = 2000;
    TIM3->CCR4 = 2000;
    for(int i=1;i<=20000000;i++);
    TIM3->CR1 |= 1<<0;

}
void TIM_UnLock(){
    PWM_All(2000);
//    printf("1\n");
    for(int i=1;i<=30000000;i++);
//    printf("2\n");
    PWM_All(1000);
    for(int i=1;i<=30000000;i++);
    PWM_All(1100);
    for (int i=1;i<=10000000;i++);
    PWM_All(1000);
}
void PWM_All(uint32_t x){
    TIM3->CCR1 = x;
    TIM3->CCR2 = x;
    TIM3->CCR3 = x;
    TIM3->CCR4 = x;
//    for(int i=1;i<=100000;i++);
    return ;
}