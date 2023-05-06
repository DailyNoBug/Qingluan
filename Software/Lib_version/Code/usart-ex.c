// STM32 USART2 (Tx PA.2, Rx PA.3) STM32F401RE NUCLEO - sourcer32@gmail.com
#include "stm32f4xx.h"
/**************************************************************************************/
void RCC_Configuration(void)
{
/* --------------------------- System Clocks Configuration -----------------*/
/* USART2 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
/* GPIOA clock enable */
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}
/**************************************************************************************/
void GPIO_Configuration(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
/*-------------------------- GPIO Configuration ----------------------------*/
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // PA.2 USART2_TX, PA.3 USART2_RX
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
/* Connect USART pins to AF */
GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
}
/**************************************************************************************/
void USART2_Configuration(void)
{
USART_InitTypeDef USART_InitStructure;
/* USARTx configuration ------------------------------------------------------*/
/* USARTx configured as follow:
- BaudRate = 115200 baud
- Word Length = 8 Bits
- One Stop Bit
- No parity
- Hardware flow control disabled (RTS and CTS signals)
- Receive and transmit enabled
*/
USART_InitStructure.USART_BaudRate = 115200;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
USART_Init(USART2, &USART_InitStructure);
USART_Cmd(USART2, ENABLE);
}
/**************************************************************************************/
void OutString(char *s)
{
while(*s)
{
while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
USART_SendData(USART2, *s++); // Send Char
}
}
/**************************************************************************************/
int main(void)
{
RCC_Configuration();
GPIO_Configuration();
USART2_Configuration();
OutString("Welcome to Nucleo F401RE\r\n");
while(1) // Don't want to exit
{
uint16_t Data;
while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET); // Wait for Char
Data = USART_ReceiveData(USART2); // Collect Char
while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
USART_SendData(USART2, Data); // Echo Char
}
}
/**************************************************************************************/
#ifdef USE_FULL_ASSERT
/**
* @brief Reports the name of the source file and the source line number
* where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
/* User can add his own implementation to report the file name and line number,
ex: printf(''Wrong parameters value: file %s on line %d\r\n'', file, line) */
while (1)
{}
}
#endif
/**************************************************************************************/