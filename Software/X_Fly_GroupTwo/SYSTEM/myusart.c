#include "myusart.h"


void MyUsart_Init(void){
	GPIO_InitTypeDef GPIO_InitTypeStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitTypeStruct.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitTypeStruct.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA, &GPIO_InitTypeStruct); //��ʼ��PA9��PA10
	
	USART_InitTypeDef USART_InitTypeStruct;
	USART_InitTypeStruct.USART_BaudRate = 9600;
	USART_InitTypeStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitTypeStruct.USART_Parity = USART_Parity_No;
	USART_InitTypeStruct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitTypeStruct);
	
	NVIC_InitTypeDef NVIC_InitTypeStruct;
	NVIC_InitTypeStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitTypeStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}
	
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == RESET) return;
	u16 data = USART_ReceiveData(USART1);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, data);
}

int fputc(int ch, FILE *f){
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, (u16)ch);
	return ch;
}
