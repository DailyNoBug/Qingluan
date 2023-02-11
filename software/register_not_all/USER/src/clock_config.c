/*
 * 时钟配置模块
 * code with UTF-8
 */
#include "../inc/clock_config.h"
/*
 * config clock source which use pll
 * PCLK1 -> 42MHz
 * PCLK2 -> 84MHz
 */
void pll_clock_config(){
    //开启HSE并且等待HSE稳定
    RCC->CR |= 1<<16;
    int round = 0;
    while(!(RCC->CR & (1<<17)) && round < MaxRound)round++;
    //设置AHB，APB2，APB1的预分频因子
    //设置AHB分频系数，设置CFGR的HPRE位 0xxx 设置为不分频
    RCC->CFGR &= ~(1<<7);
    //APB1二分频，设置CFGR的PPRE1位 100 为2分频
    RCC->CFGR &= ~(0x7<<10);
    RCC->CFGR |= (1<<12);
    //APB2不分频，设置CFGR的PPRE2位 0xx为不分频
    RCC->CFGR &= ~(0x7<<13);
    RCC->CFGR |= (0<<15);
    //设置PLL时钟来源，配置PLLCFGR寄存器
    //配置PLLSRC位 1为HSE作为锁相环输入
    RCC->PLLCFGR |= (1<<22);
    //配置锁相环MNQ参数
    RCC->PLLCFGR &= ~(0x3F);    //config PLL_M;
    RCC->PLLCFGR |= PLL_M<<0;
    RCC->PLLCFGR &= ~(0x1FF<<6);
    RCC->PLLCFGR |= PLL_N<<6;   //config PLL_N;
    RCC->PLLCFGR &= ~(0x3<<16);
    RCC->PLLCFGR |= ((PLL_P>>1)-1)<<16; //config PLL_P;
    //打开锁相环
    RCC->CR |= (1<<24);
    round = 0;
    while(!(RCC->CR & (1<<25)) && round < MaxRound)round++;
    //将PLLCK切换为系统时钟
    RCC->CFGR &= ~(0x3);
    RCC->CFGR |= (1<<1);
}