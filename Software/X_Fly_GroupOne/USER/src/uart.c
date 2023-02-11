//
// Created by 27900 on 2023/1/21.
//
#include "../inc/uart.h"
GPIO_InitTypeDef UART_Initstruct;
void UART1_GPIOInit(){
    /*
     * PA9 -> TX
     * PA10-> RX
     */
    RCC->APB2ENR |= (1<<4);
    RCC->APB2ENR |= (1<<14);
    UART_Initstruct.GPIO_Pin    = GPIO_Pin_9;
    UART_Initstruct.GPIO_Pupd   = GPIO_Pupd_UP;
    UART_Initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    UART_Initstruct.GPIO_OType  = GPIO_OType_PP;
    UART_Initstruct.GPIO_Mode   = GPIO_Mode_AF;
    GPIO_Init(GPIOA,&UART_Initstruct);
    UART_Initstruct.GPIO_Pin    = GPIO_Pin_10;
    UART_Initstruct.GPIO_Mode   = GPIO_Mode_In;
    UART_Initstruct.GPIO_Pupd   = GPIO_Pupd_UP;
    GPIO_Init(GPIOA,&UART_Initstruct);
    GPIOA->AFRH |= (0x07<<4) | (0x07<<8);
}
void USART_Init(USART_TypeDef *UARTx , uint32_t bound,uint32_t pclk){
    float temp;
    uint16_t mantissa;
    uint16_t fraction;
    temp    =(float)(pclk*1000000)/(bound*16);
    mantissa = temp;
    fraction = (temp-mantissa)*16;
    mantissa = (mantissa<<4) +fraction;

    if(UARTx == USART1){
        UART1_GPIOInit();
    }
    UARTx -> CR1 = 0x00;
    UARTx -> CR2 = 0x00;
    UARTx -> CR3 = 0x00;    //clear all
    UARTx -> BRR |= mantissa;
    //set bound
    UARTx -> CR1 |= 1<<3;
    UARTx -> CR1 |= 1<<13;  //USART enabled
    //enable TE RE bit
}
void USART_SendChar(USART_TypeDef *USARTx,char c){
    while((USARTx->SR & 0x80)==0){}
    USARTx->DR = (uint8_t)c;
}