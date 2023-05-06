//
// Created by 27900 on 2023/2/1.
//
#include "iic.h"
/**** STEPS FOLLOWED  ************
1. Enable the I2C CLOCK and GPIO CLOCK
2. Configure the I2C PINs for ALternate Functions
	a) Select Alternate Function in MODER Register
	b) Select Open Drain Output
	c) Select High SPEED for the PINs
	d) Select Pull-up for both the Pins
	e) Configure the Alternate Function in AFR Register
3. Reset the I2C
4. Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
5. Configure the clock control registers
6. Configure the rise time register
7. Program the I2C_CR1 register to enable the peripheral
*/
void I2C_RCCENR(I2C_TypeDef *I2cx){
    if(I2cx == I2C1){
        RCC->APB1ENR |= (1<<21);
    }
    else if(I2cx == I2C2){
        RCC->APB1ENR |= (1<<22);
    }
    else if(I2cx == I2C3){
        RCC->APB1ENR |= (1<<23);
    }
    return ;
}
/**
 * PB3  ->  SDA
 * PB10 ->  SCL
 */
void I2C2_GPIOInit(){
    RCC->AHB1ENR |= 1<<1;
    GPIO_InitTypeDef I2C2_Init;
    I2C2_Init.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_10;
    I2C2_Init.GPIO_Mode = GPIO_Mode_AF;
    I2C2_Init.GPIO_OType= GPIO_OType_OD;
    I2C2_Init.GPIO_Speed= GPIO_Speed_50MHz;
    I2C2_Init.GPIO_Pupd = GPIO_Pupd_UP;
    GPIO_Init(GPIOB,&I2C2_Init);
    GPIOB->AFRL |= 9<<12;
    GPIOB->AFRH |= 4<<8;
}
void I2C_Init(I2C_TypeDef *I2cx){
    I2C_RCCENR(I2cx);
    if(I2cx == I2C2){
        I2C2_GPIOInit();
    }
    //reset the i2c
    I2cx->CR1 |= (1<<15);
    I2cx->CR1 &= ~(1<<15);
    //turn of all
    I2cx->CR1 &= ~(1<<0);
    // PCLK1 FREQUENCY in MHz
    //F/S =1 DUTY=0,CCR=3;
    I2cx->CR2 |= (42<<0);

    I2cx->OAR1 |= 0x0A;

    I2cx->CCR &= ~63;
    I2cx->CCR |= (1<<15);
    I2cx->CCR |= (35<<0);

    I2cx->TRISE &= ~63;
    I2cx->TRISE = 13;

    I2cx->CR1 |= (1<<0);
    I2cx->CR1 |= (1<<10);

}
/**** STEPS FOLLOWED  ************
1. Wait for the TXE (bit 7 in SR1) to set. This indicates that the DR is empty
2. Send the DATA to the DR Register
3. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST DATA transmission
*/
void I2C_Start(I2C_TypeDef *I2cx){
//    printf("enter i2c start\n");
    while(I2cx->SR2 & (1<<1));
//    printf("Bus right!\n");
    I2cx->CR1 |= (1<<10);
    I2cx->CR1 |= (1<<8);
    while(!(I2cx->SR1 & (1<<0)));
//    printf("i2c start generate successfully\n");
}
void I2C_Write(I2C_TypeDef *I2cx,uint8_t data){
    while(!((I2cx->SR1) & (1<<7)));
    I2cx -> DR = data;
    while(!((I2cx->SR1) & (1<<2)));
}
void I2C_Address(I2C_TypeDef *I2cx,uint8_t Address){
    I2cx -> DR = Address;
//    printf("Address writed successfully\n");
    while (!(I2cx->SR1 & (1<<1)));
    uint8_t temp = I2cx->SR1 | I2cx->SR2;
}
void I2C_Stop(I2C_TypeDef *I2cx){
    I2cx -> CR1 |= (1<<9);
    return ;
}
void I2C_WriteMulti(I2C_TypeDef *I2cx,uint8_t *data,uint8_t size){
    while (!(I2cx->SR1 & (1<<7)));
    while (size){
        while (!(I2cx->SR1 & (1<<7)));
        I2cx -> DR = (uint32_t) *data++;
        size--;
    }
    while (!(I2cx->SR1 & (1<<2)));
}
void I2C_Read(I2C_TypeDef *I2cx,uint8_t Address,uint8_t *buffer,uint8_t size){
    int remind = size;
    if (size == 1){
        I2cx->DR = Address;
        while (!(I2cx->SR1 & (1<<1)));
        I2cx -> CR1 &= ~(1<<10);
        uint8_t temp = I2cx -> SR1 | I2cx ->SR2;
        I2cx -> CR1 |= (1<<9);
        while (!(I2cx->SR1 & (1<<6)));
        buffer[size-remind] = I2cx->DR;
    }else{
        I2cx->DR = Address;
        while (!(I2cx->SR1 & (1<<1)));
        uint8_t temp = I2cx->SR1 | I2cx->SR2;
        while (remind>2)
        {
            while (!(I2cx->SR1 & (1<<6)));
            buffer[size-remind] = I2cx->DR;
            I2cx->CR1 |= 1<<10;
            remind--;
        }
        while (!(I2cx->SR1 & (1<<6)));
        buffer[size-remind] = I2cx->DR;
        I2cx->CR1 &= ~(1<<10);
        I2cx->CR1 |= (1<<9);
        remind--;
        while (!(I2cx->SR1 & (1<<6)));
        buffer[size-remind] = I2cx->DR;
    }
}