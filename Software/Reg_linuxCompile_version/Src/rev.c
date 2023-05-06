//
// Created by 27900 on 2023/2/11.
//
#include "rev.h"
/**
 * use TIM2 CH1 as PPM in;(PA5)
 */
void REV_TIM_Init(void){
    //开启GPIOA，PA5
    RCC->AHB1ENR |= 1<<0;
    GPIOA->MODER |= 0x2<<10;
    GPIOA->AFRL  |= 1<<20;
    //开启TIM2时钟使能
    RCC->APB1ENR |= 1<<0;
    //CC1通道配置为输入，IC1映射到TI1上
    TIM2->CCMR1  |= 0x1<<0;
    //设置分频系数，计数器工作在1MHz下
    TIM2->PSC = 41;
    //启用更新中断和捕获中断
    TIM2->DIER |= 1<<1;
    //设置自动重装载值
    TIM2->ARR = 19999;
    NVIC_ISER1 |= 1<<28;
    //CH1启动输入捕获，捕获上升沿信号
    TIM2->CCER |= 1<<0;
    //开启定时器
    TIM2->CR1 |= 1<<0;
}
uint16_t data[9];
uint8_t  PPM_CH=0;
void TIM2_IRQHandler(void){
//    printf("111111111:   %d\n",TIM2->CCR1);
    if(TIM2->CCR1>7000||PPM_CH==9){
        PPM_CH = 0;
    }else{
        data[++PPM_CH] = TIM2->CCR1;
    }
    TIM2->CNT=0;
}