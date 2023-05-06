#include "stm32f4xx.h"  
#include "delay.h"
#include "led.h"
#include "myusart.h"
#include "i2c.h"
#include "mpu6050.h"
#include "motor.h"

int main(void)
{ 
	SystemInit();
	short Acel[3], Gyro[3], Mag[3];
	float Temp;
	
	delay_init(84);  //初始化延时函数
	LED_Init();
	LED_On();


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	MyUsart_Init();
	printf("\r\nUSART1 F401RE Test\r\n");
	RCC_ClocksTypeDef  rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	printf("ClockInfo: SYSCLK = %d HCLK = %d PCLK1 = %d PCLK2 = %d\r\n", rcc_clocks.SYSCLK_Frequency, rcc_clocks.HCLK_Frequency, rcc_clocks.PCLK1_Frequency, rcc_clocks.PCLK2_Frequency);
	I2cMaster_Init(); 
	//printf("ret = %d\r\n", MPU_Init());
	MPU6050ReadID();
	
 	MPU6050_Init();
	HMC_Init();
	
	Motor_Init(839, 1999);
	delay_ms(2000);
	TIM_SetCompare1(TIM3, 100);
	delay_ms(2000);
	//TIM_SetCompare1(TIM3, 100);
	delay_ms(2000);
	//TIM_SetCompare1(TIM3, 105); // >= 119就能够持续转动
	
	while(1){
		MPU6050ReadAcc(Acel);
		printf("Acel: %8d%8d%8d",Acel[0],Acel[1],Acel[2]);
		MPU6050ReadGyro(Gyro);
		printf("    Gyro %8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
		MPU6050_ReturnTemp(&Temp);
		printf("    Temp %8.2f", Temp);
		HMC_ReadMa(Mag);
		printf("    Mag %8d%8d%8d\r\n",Mag[0], Mag[1], Mag[2]);
		delay_ms(1000);
	}
}

