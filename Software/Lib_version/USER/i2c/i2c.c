/**
  ******************************************************************************
  * @file    i2c.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c �������������������I2C������ͨѶ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "./i2c/i2c.h"


/********************************* Defines ************************************/

#define WAIT_FOR_FLAG(flag, value, timeout, errorcode)  I2CTimeout = timeout;\
          while(I2C_GetFlagStatus(SENSORS_I2C, flag) != value) {\
            if((I2CTimeout--) == 0) return I2Cx_TIMEOUT_UserCallback(errorcode); \
          }\
  
#define CLEAR_ADDR_BIT      I2C_ReadRegister(SENSORS_I2C, I2C_Register_SR1);\
                            I2C_ReadRegister(SENSORS_I2C, I2C_Register_SR2);\
                               
/********************************* Globals ************************************/

/********************************* Prototypes *********************************/
static unsigned long ST_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue);
static unsigned long ST_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue);
static unsigned long ST_Sensors_I2C_WriteNoRegister(unsigned char Address, unsigned char RegisterAddr);
static unsigned long ST_Sensors_I2C_ReadNoRegister(unsigned char Address, unsigned short RegisterLen, unsigned char *RegisterValue);
  
					
/*******************************  Function ************************************/

/**
  * @brief  ��ʼ��I2C���ߣ�ʹ��I2Cǰ��Ҫ����
  * @param  ��
  * @retval ��
  */
void I2cMaster_Init(void) 
{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  I2C_InitTypeDef I2C_InitStructure;

    /* Enable I2Cx clock */
  //RCC_APB1PeriphClockCmd(SENSORS_I2C_RCC_CLK, ENABLE);
	RCC->APB1ENR |= 1 << 22;

  /* Enable I2C GPIO clock */
  //RCC_AHB1PeriphClockCmd(SENSORS_I2C_SCL_GPIO_CLK | SENSORS_I2C_SDA_GPIO_CLK, ENABLE);
	RCC->AHB1ENR |= 1 << 1;

  /* Configure I2Cx pin: SCL ----------------------------------------*/
//  GPIO_InitStructure.GPIO_Pin =  SENSORS_I2C_SCL_GPIO_PIN; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	//IIC2 SCL -> GPIOB Pin 10
	GPIOB->MODER &= ~(3 << 20);
	GPIOB->MODER |= 2 << 20;
	GPIOB->OTYPER |= 1 << 10;
	GPIOB->OSPEEDR |= 3 << 20;
	GPIOB->PUPDR &= ~(3 << 20);
	GPIOB->AFR[1] &= ~(15 << 8); //����Ϊʲôһ��AF4��һ��AF9��
	GPIOB->AFR[1] |= 4 << 8;
	//IIC SDA -> GPIOB Pin 3
	GPIOB->MODER &= ~(3 << 6);
	GPIOB->MODER |= 2 << 6;
	GPIOB->OTYPER |= 1 << 3;
	GPIOB->OSPEEDR |= 3 << 6;
	GPIOB->PUPDR &= ~(3 << 6);
	GPIOB->AFR[0] &= ~(15 << 12);
	GPIOB->AFR[0] |= 9 << 12;
	//GPIOB->AFR[0] |= 4 << 12; //����д�ͼ�

  /* Connect pins to Periph */
  //GPIO_PinAFConfig(SENSORS_I2C_SCL_GPIO_PORT, SENSORS_I2C_SCL_GPIO_PINSOURCE, GPIO_AF_I2C2);  
  //GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure I2Cx pin: SDA ----------------------------------------*/
  //GPIO_InitStructure.GPIO_Pin = SENSORS_I2C_SDA_GPIO_PIN; 

  /* Connect pins to Periph */
 // GPIO_PinAFConfig(SENSORS_I2C_SDA_GPIO_PORT, SENSORS_I2C_SDA_GPIO_PINSOURCE, GPIO_AF9_I2C2);  
  //GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);  
  
  //I2C_DeInit(SENSORS_I2C);
//  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//  I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
//  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  //IIC����
  //I2C2->CR1 |= 1; //IIC2ʹ��
  I2C2->CR1 &= ~(1 << 1); //IICģʽ
  I2C2->CCR |= 1 << 15; //����ģʽ
  I2C2->CCR |= ~(1 << 14); //ռ�ձ�low/high=2 
  I2C2->OAR1 |= 0x0A; //������ַΪ0x5(0x0A = (1010)_2�����λ��7��ַ��Ч)
  I2C2->CR2 |= 42; //��������ʱ��Ƶ�ʣ���APB1ʱ��Ƶ��42Mhz
  I2C2->TRISE &= ~63; //����ģʽ��Trise = 42 * 300 / 1000 + 1����׼ģʽ��Trise = 42 + 1
  I2C2->TRISE |= 13;
  I2C2->CCR &= ~(0xFFF);
  I2C2->CCR |= 35; //CCR����Ϊ35(Ŀǰ��ռ�ձ���CCR = T_high / T_PCLK1)
  
  I2C2->CR1 |= 1; //IIC2ʹ��
  I2C2->CR1 |= 1 << 10; //����Ӧ��
  
    
  /* Enable the I2C peripheral */
  //I2C_Cmd(SENSORS_I2C, ENABLE);  
    
  /* Initialize the I2C peripheral */
  //I2C_Init(SENSORS_I2C, &I2C_InitStructure);
  
  return;
}

/**
  * @brief  ��ʱ�ص����������I2C��־��ʱ���ñ���������ʼ��I2C���¼�⡣
  * @param  ������룬ÿ�������ж����ı�ţ����ڲ��Ҵ���
  * @retval Ĭ�Ϸ���1����ʾI2C������
  */
static uint32_t I2Cx_TIMEOUT_UserCallback(char value)
{
  
  /* The following code allows I2C error recovery and return to normal communication
     if the error source doesn�t still exist (ie. hardware issue..) */
  I2C_InitTypeDef I2C_InitStructure;
  
  I2C_GenerateSTOP(SENSORS_I2C, ENABLE);
  I2C_SoftwareResetCmd(SENSORS_I2C, ENABLE);
  I2C_SoftwareResetCmd(SENSORS_I2C, DISABLE);
  
   I2C_DeInit(SENSORS_I2C);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    
  /* Enable the I2C peripheral */
  I2C_Cmd(SENSORS_I2C, ENABLE);  
    
  /* Initialize the I2C peripheral */
  I2C_Init(SENSORS_I2C, &I2C_InitStructure);
	
	I2C_ERROR("I2C callback error code = %d\r\n",value);

  return 1;
}

/**
  * @brief  д�Ĵ���(��γ���)�������ṩ���ϲ�Ľӿ�
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ
	* @param len��д��ĳ���
	*	@param data_ptr:ָ��Ҫд�������
  * @retval ����Ϊ0��������Ϊ��0
  */
int Sensors_I2C_WriteRegister(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len, 
                                        const unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();
                              
tryWriteAgain:  
  ret = 0;
  ret = ST_Sensors_I2C_WriteRegister( slave_addr, reg_addr, len, data_ptr); 

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;
    
    Delay(retry_in_mlsec);
    goto tryWriteAgain;
  }
  return ret;  
}

/**
  * @brief  ���Ĵ���(��γ���)�������ṩ���ϲ�Ľӿ�
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ
	* @param len��Ҫ��ȡ�ĳ���
	*	@param data_ptr:ָ��Ҫ�洢���ݵ�ָ��
  * @retval ����Ϊ0��������Ϊ��0
  */
int Sensors_I2C_ReadRegister(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len, 
                                       unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();
  
tryReadAgain:  
  ret = 0;
  ret = ST_Sensors_I2C_ReadRegister( slave_addr, reg_addr, len, data_ptr);

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;
    
    Delay(retry_in_mlsec);
    goto tryReadAgain;
  } 
  return ret;
}

int IIC_Check_SR1Status(u16 bit) {
	return (I2C2->SR1 >> bit) & 1;
}

int IIC_Check_SR2Status(u16 bit) {
	return (I2C2->SR2 >> bit) & 1;
}

/**
  * @brief  д�Ĵ���(���γ���)�����ǵײ�I2C�ӿ�
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ
	* @param len��д��ĳ���
	*	@param data_ptr:ָ��Ҫд�������
  * @retval ����Ϊ0��������Ϊ��0
  */
static unsigned long ST_Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue)
{
  uint32_t  result = 0, read_temp = 0;
  uint32_t  i = 0; // i = RegisterLen;
  __IO uint32_t  I2CTimeout = I2Cx_LONG_TIMEOUT;
  
//  RegisterValue = RegisterValue + (RegisterLen - 1);

  /* Wait for the busy flag to be cleared */
  //WAIT_FOR_FLAG (I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 1);
	while(IIC_Check_SR2Status(1)){};
  
  /* Start the config sequence */
  //I2C_GenerateSTART(SENSORS_I2C, ENABLE);
	I2C2->CR1 |= 1 << 8;

  /* Wait for the start bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 2);
	while(!IIC_Check_SR1Status(0)){};

  /* Transmit the slave address and enable writing operation */
  //I2C_Send7bitAddress(SENSORS_I2C, (Address<<1), I2C_Direction_Transmitter);
	I2C2->DR = Address << 1;
  
  /* Wait for address bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 3);
	while(!IIC_Check_SR1Status(1)){};	
  
  /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
  //CLEAR_ADDR_BIT
	read_temp = I2C2->SR1; read_temp = I2C2->SR2;
  
  /* Wait for TXE bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 4);
	while(!IIC_Check_SR1Status(7)){};

  /* Transmit the first address for write operation */
  //I2C_SendData(SENSORS_I2C, RegisterAddr);
	I2C2->DR = RegisterAddr;

  
  for(i=0; i< RegisterLen; i++)
  {
    /* Wait for address bit to be set */
    //WAIT_FOR_FLAG (I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 5);
	  while(!IIC_Check_SR1Status(7)){};
  
    /* Prepare the register value to be sent */
    //I2C_SendData(SENSORS_I2C, RegisterValue[i]);
	  I2C2->DR = RegisterValue[i];
	
	//Wait for BIT
	while(!IIC_Check_SR1Status(2));
  }  
   
//  /* Wait for BTF bit to be set */
//  WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 6);
  
  /* End the configuration sequence */
  //I2C_GenerateSTOP(SENSORS_I2C, ENABLE);  
  I2C2->CR1 |= 1 << 9;
  
  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return result;  
}


/**
  * @brief  ���Ĵ���(���γ���)�����ǵײ�I2C�ӿ�
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ
	* @param len��Ҫ��ȡ�ĳ���
	*	@param data_ptr:ָ��Ҫ�洢���ݵ�ָ��
  * @retval ����Ϊ0��������Ϊ��0
  */
static unsigned long ST_Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue)
{
  uint32_t i=0, result = 0, read_temp = 0;
  __IO uint32_t  I2CTimeout = I2Cx_LONG_TIMEOUT;
   
  /* Wait for the busy flag to be cleared */
  //WAIT_FOR_FLAG (I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 7);
	while(IIC_Check_SR2Status(1)){};
  
  /* Start the config sequence */
  //I2C_GenerateSTART(SENSORS_I2C, ENABLE);
	I2C2->CR1 |= 1 << 8;

  /* Wait for the start bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 8);
	while(!IIC_Check_SR1Status(0)){};
  
  /* Transmit the slave address and enable writing operation */
  //I2C_Send7bitAddress(SENSORS_I2C, (Address<<1), I2C_Direction_Transmitter);
	I2C2->DR = Address << 1;

  /* Wait for the address bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 9);
	while(!IIC_Check_SR1Status(1)){};

  /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
	//CLEAR_ADDR_BIT;
	read_temp = I2C2->SR1; read_temp = I2C2->SR2;
  
  /* Wait for TXE bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 10);
	while(!IIC_Check_SR1Status(7)){};
  
  /* Transmit the register address to be read */
  //I2C_SendData(SENSORS_I2C, RegisterAddr);
	I2C2->DR = RegisterAddr;
  
  /* Wait for TXE bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 11);  
	while(!IIC_Check_SR1Status(7)){};
		
  /*!< Send START condition a second time */  
  //I2C_GenerateSTART(SENSORS_I2C, ENABLE);
	I2C2->CR1 |= 1 << 8;
  
  /* Wait for the start bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 12);
	while(!IIC_Check_SR1Status(0)){};
  
  /*!< Send address for read */
  //I2C_Send7bitAddress(SENSORS_I2C, (Address<<1), I2C_Direction_Receiver); 
	I2C2->DR = Address << 1 | 1;
  
  /* Wait for the address bit to be set */
  //WAIT_FOR_FLAG (I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 13);
	while(!IIC_Check_SR1Status(1)){};
	
	if (RegisterLen == 1) {
		//Clear the ACK bit
		I2C2->CR1 &= ~(1 << 10);
		//CLEAR_ADDR_BIT;
		read_temp = I2C2->SR1; read_temp = I2C2->SR2;
		//Stop I2C
		I2C2->CR1 |= 1 << 9;
		//Wait for RxNE to set
		while(!IIC_Check_SR1Status(6));
		//Read data
		RegisterValue[0] = I2C2->DR;
	} else {
		//CLEAR_ADDR_BIT;
		read_temp = I2C2->SR1; read_temp = I2C2->SR2;
		
		while (i < RegisterLen - 2) {
			//Wait for RxNE to set
			while(!IIC_Check_SR1Status(6));
			//Read data
			RegisterValue[i++] = I2C2->DR;
			//Set the ACK bit(Not necessary?)
			I2C2->CR1 |= 1 << 10;
		}
		//Wait for RxNE to set
		while(!IIC_Check_SR1Status(6));
		//Read the second last data
		RegisterValue[i++] = I2C2->DR;
		
		//Clear the ACK bit
		I2C2->CR1 &= ~(1 << 10);
		//Stop I2C
		I2C2->CR1 |= 1 << 9;
		
		//Wait for RxNE to set
		while(!IIC_Check_SR1Status(6));
		//Read the last data
		RegisterValue[i++] = I2C2->DR;
	}
	//Clear BTF flag
	I2C2->SR1 &= ~(1 << 2);
	//Wait for the busy flag to be cleared
	while(IIC_Check_SR2Status(1)){};
	//Re-Enable the ACK bit
	I2C2->CR1 |= 1 << 10;
  
//  if (RegisterLen == 1) 
//  {
//    /*!< Disable Acknowledgment */
//    I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
//    
//    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
//    CLEAR_ADDR_BIT;
//    
//    /*!< Send STOP Condition */
//    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);
//    
//    /* Wait for the RXNE bit to be set */
//    WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 14);
//    
//    RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
//  } 
//  else if( RegisterLen == 2) 
//  {
//     /*!< Disable Acknowledgment */
//    I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
//    
//   /* Set POS bit */ 
//   SENSORS_I2C->CR1 |= I2C_CR1_POS;
//   
//   /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
//   CLEAR_ADDR_BIT; 
//   
//   /* Wait for the buffer full bit to be set */
//   WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 15);
//   
//   /*!< Send STOP Condition */
//   I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

//   /* Read 2 bytes */
//   RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
//   RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
//  } 
//  else if( RegisterLen == 3)
//  {
//    CLEAR_ADDR_BIT;
//    
//    /* Wait for the buffer full bit to be set */
//    WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);
//    /*!< Disable Acknowledgment */
//    I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
//    /* Read 1 bytes */
//    RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
//    /*!< Send STOP Condition */
//    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);        
//    /* Read 1 bytes */
//    RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
//    /* Wait for the buffer full bit to be set */
//    WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);
//    /* Read 1 bytes */
//    RegisterValue[2] = I2C_ReceiveData(SENSORS_I2C);  
//  }  
//  else /* more than 2 bytes */
//  { 
//    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
//    CLEAR_ADDR_BIT;
//    
//    for(i=0; i<(RegisterLen); i++)
//    {
//      if(i==(RegisterLen-3))
//      {
//        /* Wait for the buffer full bit to be set */
//        WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);
//        
//        /*!< Disable Acknowledgment */
//        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
//        
//        /* Read 1 bytes */
//        RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);
//        
//        /*!< Send STOP Condition */
//        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);        
//        
//        /* Read 1 bytes */
//        RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);
//        
//        /* Wait for the buffer full bit to be set */
//        WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);
//        
//        /* Read 1 bytes */
//        RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);  
//        goto endReadLoop;
//      }
//            
//      /* Wait for the RXNE bit to be set */
//      WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 18);
//      RegisterValue[i] = I2C_ReceiveData(SENSORS_I2C); 
//    }   
//  } 
//  
//endReadLoop:  
//  /* Clear BTF flag */
//  I2C_ClearFlag(SENSORS_I2C, I2C_FLAG_BTF);
//  /* Wait for the busy flag to be cleared */
//  WAIT_FOR_FLAG (I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 19);  
//  /*!< Re-Enable Acknowledgment to be ready for another reception */
//  I2C_AcknowledgeConfig(SENSORS_I2C, ENABLE);
//  //Disable POS -- TODO
//  SENSORS_I2C->CR1 &= ~I2C_CR1_POS;  
//     
//  /* Return the byte read from sensor */
  return result;
}



/**
  * @brief  д����(��γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��������ĺ��������������
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ��Ҫд������
  * @retval ����Ϊ0��������Ϊ��0
  */
int Sensors_I2C_WriteNoRegister(unsigned char slave_addr,
                                        unsigned char reg_addr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();
                              
tryWriteAgain:  
  ret = 0;
  ret = ST_Sensors_I2C_WriteNoRegister( slave_addr, reg_addr); 

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;
    
    Delay(retry_in_mlsec);
    goto tryWriteAgain;
  }
  return ret;  
}


/**
  * @brief  ������(��γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��ȡ����(����Ҫ���ͼĴ�����ַ)
	* @param  slave_addr: �ӻ���ַ
	* @param 	RegisterLen:���ݳ���
	*	@param	RegisterValue��ָ��洢���ݵ�ָ��
  * @retval ����Ϊ0��������Ϊ��0
  */
int Sensors_I2C_ReadNoRegister(unsigned char slave_addr,
                                       unsigned short len, 
                                       unsigned char *data_ptr)
{
  char retries=0;
  int ret = 0;
  unsigned short retry_in_mlsec = Get_I2C_Retry();
  
tryReadAgain:  
  ret = 0;
  ret = ST_Sensors_I2C_ReadNoRegister( slave_addr,len, data_ptr);

  if(ret && retry_in_mlsec)
  {
    if( retries++ > 4 )
        return ret;
    
    Delay(retry_in_mlsec);
    goto tryReadAgain;
  } 
  return ret;
}




/**
  * @brief  д����(���γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��������ĺ��������������
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ��Ҫд������
  * @retval ����Ϊ0��������Ϊ��0
  */
static unsigned long ST_Sensors_I2C_WriteNoRegister(unsigned char Address, unsigned char RegisterAddr)
{
  uint32_t  result = 0;
  __IO uint32_t  I2CTimeout = I2Cx_LONG_TIMEOUT;
  

  /* Wait for the busy flag to be cleared */
  WAIT_FOR_FLAG (I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 1);
  
  /* Start the config sequence */
  I2C_GenerateSTART(SENSORS_I2C, ENABLE);

  /* Wait for the start bit to be set */
  WAIT_FOR_FLAG (I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 2);

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(SENSORS_I2C, (Address<<1), I2C_Direction_Transmitter);
  
  /* Wait for address bit to be set */
  WAIT_FOR_FLAG (I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 3);
  
  /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
  CLEAR_ADDR_BIT
  
  /* Wait for address bit to be set */
  WAIT_FOR_FLAG (I2C_FLAG_TXE, SET, I2Cx_FLAG_TIMEOUT, 4);

  /* Transmit the first address for write operation */
  I2C_SendData(SENSORS_I2C, RegisterAddr);

   
  /* Wait for address bit to be set */
  WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 6);
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(SENSORS_I2C, ENABLE);  
  
  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return result;  
}


/**
  * @brief  ������(���γ���)�����ǵײ�I2C�ӿڣ�ר����ֻ��IIC�豸��ȡ����(����Ҫ���ͼĴ�����ַ)
	* @param  slave_addr: �ӻ���ַ
	* @param 	RegisterLen:���ݳ���
	*	@param	RegisterValue��ָ��洢���ݵ�ָ��
  * @retval ����Ϊ0��������Ϊ��0
  */
static unsigned long ST_Sensors_I2C_ReadNoRegister(unsigned char Address, unsigned short RegisterLen, unsigned char *RegisterValue)
{
  uint32_t i=0, result = 0;
  __IO uint32_t  I2CTimeout = I2Cx_LONG_TIMEOUT;
   
  /* Wait for the busy flag to be cleared */
  WAIT_FOR_FLAG (I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 7);
  

  /*!< Send START condition a second time */  
  I2C_GenerateSTART(SENSORS_I2C, ENABLE);
  
  /* Wait for the start bit to be set */
  WAIT_FOR_FLAG (I2C_FLAG_SB, SET, I2Cx_FLAG_TIMEOUT, 12);
  
  /*!< Send address for read */
  I2C_Send7bitAddress(SENSORS_I2C, (Address<<1), I2C_Direction_Receiver);  
  
  /* Wait for the start bit to be set */
  WAIT_FOR_FLAG (I2C_FLAG_ADDR, SET, I2Cx_FLAG_TIMEOUT, 13);
  
  if (RegisterLen == 1) 
  {
    /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
    
    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
    CLEAR_ADDR_BIT;
    
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);
    
    /* Wait for the RXNE bit to be set */
    WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 14);
    
    RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
  } 
  else if( RegisterLen == 2) 
  {
     /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
    
   /* Set POS bit */ 
   SENSORS_I2C->CR1 |= I2C_CR1_POS;
   
   /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
   CLEAR_ADDR_BIT; 
   
   /* Wait for the buffer full bit to be set */
   WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 15);
   
   /*!< Send STOP Condition */
   I2C_GenerateSTOP(SENSORS_I2C, ENABLE);

   /* Read 2 bytes */
   RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
   RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
  } 
  else if( RegisterLen == 3)
  {
    CLEAR_ADDR_BIT;
    
    /* Wait for the buffer full bit to be set */
    WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);
    /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
    /* Read 1 bytes */
    RegisterValue[0] = I2C_ReceiveData(SENSORS_I2C);
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(SENSORS_I2C, ENABLE);        
    /* Read 1 bytes */
    RegisterValue[1] = I2C_ReceiveData(SENSORS_I2C);
    /* Wait for the buffer full bit to be set */
    WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);
    /* Read 1 bytes */
    RegisterValue[2] = I2C_ReceiveData(SENSORS_I2C);  
  }  
  else /* more than 2 bytes */
  { 
    /* clear the ADDR interrupt bit  - this is done by reading SR1 and SR2*/
    CLEAR_ADDR_BIT;
    
    for(i=0; i<(RegisterLen); i++)
    {
      if(i==(RegisterLen-3))
      {
        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG (I2C_FLAG_BTF, SET, I2Cx_FLAG_TIMEOUT, 16);
        
        /*!< Disable Acknowledgment */
        I2C_AcknowledgeConfig(SENSORS_I2C, DISABLE);
        
        /* Read 1 bytes */
        RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);
        
        /*!< Send STOP Condition */
        I2C_GenerateSTOP(SENSORS_I2C, ENABLE);        
        
        /* Read 1 bytes */
        RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);
        
        /* Wait for the buffer full bit to be set */
        WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 17);
        
        /* Read 1 bytes */
        RegisterValue[i++] = I2C_ReceiveData(SENSORS_I2C);  
        goto endReadLoop;
      }
            
      /* Wait for the RXNE bit to be set */
      WAIT_FOR_FLAG (I2C_FLAG_RXNE, SET, I2Cx_FLAG_TIMEOUT, 18);
      RegisterValue[i] = I2C_ReceiveData(SENSORS_I2C); 
    }   
  } 
  
endReadLoop:  
  /* Clear BTF flag */
  I2C_ClearFlag(SENSORS_I2C, I2C_FLAG_BTF);
  /* Wait for the busy flag to be cleared */
  WAIT_FOR_FLAG (I2C_FLAG_BUSY, RESET, I2Cx_LONG_TIMEOUT, 19);  
  /*!< Re-Enable Acknowledgment to be ready for another reception */
  I2C_AcknowledgeConfig(SENSORS_I2C, ENABLE);
  //Disable POS -- TODO
  SENSORS_I2C->CR1 &= ~I2C_CR1_POS;  
     
  /* Return the byte read from sensor */
  return result;
}



static unsigned short RETRY_IN_MLSEC  = 55; //55 -> 555


/**
  * @brief  �������Եȴ�������
  * @param  ��
  * @retval ��
  */
void Set_I2C_Retry(unsigned short ml_sec)
{
  RETRY_IN_MLSEC = ml_sec;
}

/**
  * @brief  ��ȡ���Եȴ�������
  * @param  ��
  * @retval ��
  */
unsigned short Get_I2C_Retry(void)
{
  return RETRY_IN_MLSEC;
}