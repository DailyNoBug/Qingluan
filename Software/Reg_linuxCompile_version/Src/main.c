#include "clock_config.h"
#include "gpio.h"
#include "uart.h"
#include "mpu6050.h"
#include "iic.h"
#include "pwm.h"
#include "rev.h"
#include <stdio.h>

extern float Ax,Ay,Az;
extern uint16_t data[9];
#if 1

#pragma import(__use_no_semihosting)

/* 定义 _sys_exit() 以避免使用半主机模式 */
struct __FILE
{
    int handle;
};

FILE __stdout;
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART6->SR & 0X40)==0);
    USART6->DR = (uint8_t) ch;
    return ch;
}
#endif
/*
 * init PA5
 */
void debug_init(){
    GPIO_InitTypeDef InitStruct;
    InitStruct.GPIO_Pin     = GPIO_Pin_5;
    InitStruct.GPIO_Mode    = GPIO_Mode_Out;
    InitStruct.GPIO_OType   = GPIO_OType_PP;
    InitStruct.GPIO_Speed   = GPIO_Speed_2MHz;
    InitStruct.GPIO_Pupd    = GPIO_Pupd_NOPULL;
    GPIO_Init(GPIOA,&InitStruct);
}
int main(){
    pll_clock_config();
    USART_Init(USART6,9600,42);
    printf("UART inited\n");
    I2C_Init(I2C2);
    printf("I2C inited\n");
    TIM_Init(19999,41);
//    for(int i=1;i<=10000000;i++);
    TIM_UnLock();
    printf("Unlock successful\n");
    REV_TIM_Init();
    printf("REV inited!\n");
    MPU6050_Init();
    printf("MPU6050 inited\n");
    USART_SendChar(USART6,'c');
    while(1){
        MPU6050_Read_Accel();
        printf("%f %f %f\n",Ax,Ay,Az);
        MPU6050_Read_Gyro();
        printf("%f %f %f\n",Ax,Ay,Az);
        MPU6050_Read_temp();
        printf("%f\n",Ax);
        for(int i=1;i<=8;i++)
        {
            printf("%d        %d\n",i,data[i]);
        }
        if(data[7]>1200&&data[7]){
            //遥控器未开启
            PWM_All(1000);
        }
        else if(data[2]==0||3000-data[2]>2000){
            PWM_All(1000);
        }
        else{
            PWM_All(3000-data[2]);
        }
//        for(int i=1;i<=10000000;i++);
//        GPIO_ReSetBits(GPIOA,GPIO_Pin_5);
    }
}