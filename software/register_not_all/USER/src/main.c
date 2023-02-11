#include "../inc/clock_config.h"
#include "../inc/gpio.h"
#include "../inc/uart.h"
#include "../inc/mpu6050.h"
#include "../inc/iic.h"
#include "../inc/pwm.h"
#include <stdio.h>

extern float Ax,Ay,Az;
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef’ d in stdio.h. */
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
int _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40)==0);
    USART1->DR = (uint8_t) ch;
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
    USART_Init(USART1,115200,42);
    printf("UART inited\n");
//    I2C_Init(I2C2);
//    printf("I2C inited\n");
//    MPU6050_Init();
//    printf("MPU6050 inited\n");
    TIM_Init(19999,41);
    for(int i=1;i<=5000000;i++);
    printf("Unlock\n");
    TIM_UnLock();
    while(1){
//        MPU6050_Read_Accel();
//        printf("%f %f %f\n",Ax,Ay,Az);
//        MPU6050_Read_Gyro();
//        printf("%f %f %f\n",Ax,Ay,Az);
//        MPU6050_Read_temp();
//        printf("%f\n",Ax);
//        USART_SendChar(USART1,'p');
//        GPIO_SetBits(GPIOA,GPIO_Pin_5);
        PWM_All(1800);
        for(int i=1;i<=10000000;i++);
//        GPIO_ReSetBits(GPIOA,GPIO_Pin_5);
    }
}