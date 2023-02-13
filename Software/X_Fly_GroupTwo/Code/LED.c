#include "stm32f4xx.h"

int main() {
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitTypeStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	
//	RCC->AHB1ENR |= 1<<0;
//	GPIOA->MODER |= 1<<10;
//	GPIOA->OSPEEDR |= 1<<11;
//	GPIOA->PUPDR |= 1<<10;
//	GPIOA->ODR |= 1<<5;
	while(1);
}
