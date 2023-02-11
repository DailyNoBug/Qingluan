//
//  Created by LSC on 2023/1/20.
//  code with utf-8
//  寄存器定义文件
#ifndef STM32_REGISTER_STM32F401RET6_H
#define STM32_REGISTER_STM32F401RET6_H

#define __IO volatile
typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;
#define MaxRound 1000
#define PLL_M 8
#define PLL_N 84
#define PLL_P 2
/*
 * 基地址定义部分
 */
#define RCC_BASE    0x40023800
#define GPIOA_BASE  0x40020000
#define GPIOB_BASE  0x40020400
#define GPIOC_BASE  0x40020800
#define GPIOD_BASE  0x40020C00
#define GPIOE_BASE  0x40021000

#define USART1_BASE 0x40011000
#define USART2_BASE 0x40004400
#define USART6_BASE 0x40011400

#define I2C1_BASE   0x40005400
#define I2C2_BASE   0x40005800
#define I2C3_BASE   0x40005C00

#define TIM1_BASE   0x40010000
#define TIM2_BASE   0x40000000
#define TIM3_BASE   0x40000400
#define TIM4_BASE   0x40000800
#define TIM5_BASE   0x40000C00

#define EXTI_BASE   0x40013C00
#define NVIC_BASE   0xe000e000
#define NVIC_ISER1  *(uint32_t *)(NVIC_BASE+0x100)
/*
 * RCC时钟部分寄存器定义
 */
typedef struct {
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    __IO uint32_t RES1;
    __IO uint32_t RES2;
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t RES3;
    __IO uint32_t RES4;
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    __IO uint32_t RES5;
    __IO uint32_t RES6;
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t RES7;
    __IO uint32_t RES8;
    __IO uint32_t AHB1LPENR;
    __IO uint32_t AHB2LPENR;
    __IO uint32_t RES9;
    __IO uint32_t RES10;
    __IO uint32_t APB1LPENR;
    __IO uint32_t APB2LPENR;
    __IO uint32_t RES11;
    __IO uint32_t RES12;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    __IO uint32_t RES13;
    __IO uint32_t RES14;
    __IO uint32_t SSCGR;
    __IO uint32_t PLLI2SCFGR;
    __IO uint32_t RES15;
    __IO uint32_t DCKCFGR;
} RCC_TypeDef;
/*
 * GPIO部分的寄存器定义
 */
typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint16_t BSRRL;
    __IO uint16_t BSRRH;
    __IO uint32_t LCKR;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
}GPIO_TypeDef;
/*
 * 定义引脚模式的枚举定义
 */
typedef enum {
    GPIO_Mode_In    = 0x00,
    GPIO_Mode_Out   = 0x01,
    GPIO_Mode_AF    = 0x02,
    GPIO_Mode_AN    = 0x03
}GPIOMode_TypeDef;              //枚举引脚模式
typedef enum {
    GPIO_OType_PP   = 0x00,
    GPIO_OType_OD   = 0x01
}GPIOOType_TypeDef;             //枚举输出模式
typedef enum {
    GPIO_Speed_2MHz = 0x00,
    GPIO_Speed_25MHz= 0x01,
    GPIO_Speed_50MHz= 0x02,
    GPIO_Speed_100MHz=0x03
}GPIOSpeed_TypeDef;             //枚举GPIO速率
typedef enum {
    GPIO_Pupd_NOPULL    = 0x00,
    GPIO_Pupd_UP        = 0x01,
    GPIO_Pupd_DOWN      = 0x02
}GPIOPupd_TypeDef;              //枚举GPIO上拉下拉
/*
 * 初始化GPIO所需要的信息
 */
typedef struct {
    uint32_t            GPIO_Pin;       //需要配置的GPIO引脚
    GPIOMode_TypeDef    GPIO_Mode;      //GPIO引脚的工作模式
    GPIOSpeed_TypeDef   GPIO_Speed;     //GPIO引脚的工作速率
    GPIOOType_TypeDef   GPIO_OType;     //GPIO引脚的输出类型
    GPIOPupd_TypeDef    GPIO_Pupd;      //选择GPIO引脚的上拉/下拉模式
}GPIO_InitTypeDef;
/*
 * 初始化iic所需要的寄存器
 */
typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t OAR1;
    __IO uint32_t OAR2;
    __IO uint32_t DR;
    __IO uint32_t SR1;
    __IO uint32_t SR2;
    __IO uint32_t CCR;
    __IO uint32_t TRISE;
    __IO uint32_t FLTR;
}I2C_TypeDef;

typedef struct {
    __IO uint32_t    SR;
    __IO uint32_t    DR;
    __IO uint32_t    BRR;
    __IO uint32_t    CR1;
    __IO uint32_t    CR2;
    __IO uint32_t    CR3;
    __IO uint32_t    GTPR;
}USART_TypeDef;
/**
 * TIM2 - TIM5 寄存器定义
 */
typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    __IO uint32_t RES1;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
    __IO uint32_t RES2;
    __IO uint32_t DCR;
    __IO uint32_t DMAR;
    __IO uint32_t TIM2_OR;
    __IO uint32_t TIM5_OR;
}TIM_TypeDef;
/**
 * EXIT 寄存器定义
 */
 typedef struct {
     __IO uint32_t IMR;
     __IO uint32_t EMR;
     __IO uint32_t RTSR;
     __IO uint32_t FTSR;
     __IO uint32_t SWIER;
     __IO uint32_t PR;
 }EXTI_TypeDef;

#define GPIO_Pin_0  ((uint16_t) 0x0001)
#define GPIO_Pin_1  ((uint16_t) 0x0002)
#define GPIO_Pin_2  ((uint16_t) 0x0004)
#define GPIO_Pin_3  ((uint16_t) 0x0008)
#define GPIO_Pin_4  ((uint16_t) 0x0010)
#define GPIO_Pin_5  ((uint16_t) 0x0020)
#define GPIO_Pin_6  ((uint16_t) 0x0040)
#define GPIO_Pin_7  ((uint16_t) 0x0080)
#define GPIO_Pin_8  ((uint16_t) 0x0100)
#define GPIO_Pin_9 ((uint16_t) 0x0200)
#define GPIO_Pin_10 ((uint16_t) 0x0400)
#define GPIO_Pin_11 ((uint16_t) 0x0800)
#define GPIO_Pin_12 ((uint16_t) 0x1000)
#define GPIO_Pin_13 ((uint16_t) 0x2000)
#define GPIO_Pin_14 ((uint16_t) 0x4000)
#define GPIO_Pin_15 ((uint16_t) 0x8000)
#define GPIO_Pin_all ((uint16_t)    0xFFFF)
//base point part
#define RCC     ((RCC_TypeDef *)    RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)   GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)   GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)   GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)   GPIOD_BASE)
#define GPIOE   ((GPIO_TypeDef *)   GPIOE_BASE)
#define USART1  ((USART_TypeDef *)  USART1_BASE)

#define I2C1    ((I2C_TypeDef *)    I2C1_BASE)
#define I2C2    ((I2C_TypeDef *)    I2C2_BASE)
#define I2C3    ((I2C_TypeDef *)    I2C3_BASE)

#define TIM1    ((TIM_TypeDef *)    TIM1_BASE)
#define TIM2    ((TIM_TypeDef *)    TIM2_BASE)
#define TIM3    ((TIM_TypeDef *)    TIM3_BASE)
#define TIM4    ((TIM_TypeDef *)    TIM4_BASE)
#define TIM5    ((TIM_TypeDef *)    TIM5_BASE)
#define EXTI    ((EXTI_TypeDef*)    EXTI_BASE)
#endif //STM32_REGISTER_STM32F401RET6_H