#include "oled.h"

//EEPROM 是一种掉电后数据不丢失的存储器
//型号：AT24C02 的SCL--PB6   SDA--PB7  IIC1
//address 高4位1010b 低三位A0/a1/a2 
			// 1010000 0x50
//读写方向位 0 写 1010 0000 0xA0
//			 1 读 1010 0001 0xA1

uint16_t EEPROM_ADDRESS;
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD ; //GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 /*| GPIO_Pin_7 */;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
}

static void I2C_Mpde_Config(void)
{
	I2C_InitTypeDef I2C_InitStruct ;
	I2C_InitStruct.I2C_Mode 		=  I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1  =  I2Cx_OWN_ADDR;
	I2C_InitStruct.I2C_DutyCycle	= I2C_DutyCycle_2; //占空比
	I2C_InitStruct.I2C_ClockSpeed	= I2C_Speed;
	I2C_InitStruct.I2C_Ack			= I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C1,&I2C_InitStruct);
	
	//使能I2C
	I2C_Cmd(I2C1, ENABLE);
}


void I2C_EEPROM_Init(void)
{
	I2C_GPIO_Config();
	I2C_Mpde_Config();
	EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
}

/* 向EEPROM写入一个字节的数据 */
uint32_t I2C_EE_Write_Byte(u8 *pBuf, u8 WriteAddr)
{
	//* 产生 I2C 起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	//设置超时等待时间
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV5事件并清除标志
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(0);
	}
	
	
	//发送EEPROM设备地址
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//检测EV6事件并清除标志
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(1);		
	}
	
	
	/* 发送要写入的 EEPROM 内部地址 (即 EEPROM 内部存储器的地址) 
			上面的是指 I2C 总线设备的独立地址	    
		也可理解为 EEPROM 内存的地址或 I2C 设备的寄存器地址；  */
	I2C_SendData(I2C1, WriteAddr);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV8事件并清除标志
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(2);
	}	
	
	
	//Sends a data byte through the I2Cx peripheral.
	I2C_SendData(I2C1,*pBuf);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//检测EV8事件并清除标志
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(3);
	}	

	I2C_GenerateSTOP(I2C1,ENABLE);
	
	return 1;
}

/**
  * @brief  Wait for EEPROM Standby state (就绪)
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
	vu16 SR1_Tmp = 0;

	do
	{
		/* Send START condition */
		I2C_GenerateSTART(I2C1, ENABLE);
		/* Read I2C1 SR1 register */
		SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
		/* Send EEPROM address for write */
		I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	}

	// SR1 位 1 ADDR：1 表示地址发送成功，0 表示地址发送没有结束
	// 等待地址发送成功
	while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

	/* Clear AF flag */
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	/* STOP condition */    
    I2C_GenerateSTOP(I2C1, ENABLE); 
}

/**
	将缓冲区中的数据写到 I2C EEPROM 中，采用单字节写入的方式速度比页写入慢
	NumByteToWrite: 写的字节数
**/
uint8_t I2C_EE_Bytes_write(uint8_t* pBuf,uint8_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	uint8_t res;
	for(i=0; i<NumByteToWrite;i++)
	{
		I2C_EE_WaitEepromStandbyState();
		res = I2C_EE_Write_Byte(pBuf++,WriteAddr++);
	}
	return res;
}


/* EEPROM的页写入 
在以上的数据通讯中，每写入一个数据都需要向 EEPROM 发送写入的地址，我们希望向连续地
址写入多个数据的时候，只要告诉 EEPROM 第一个内存地址 address1，
后面的数据按次序写入到address2、address3?这样可以节省通讯的时间，加快速度。
为应对这种需求，EEPROM 定义了一种页写入时序..

	AT24C02 型号的芯片页写入时序最多可以一次发送 8 个数据
	(即 n = 8 )，该值也称为页大小
	NumByteToWrite: 要写的字节数要求 NumByToWrite 小于页大小
*/
uint8_t I2C_EE_PageWrite(uint8_t* pBuf, uint8_t WriteAddr,uint8_t NumByteToWrite)
{
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY) )
	{
		if( (I2CTimeout--) == 0 )
			return I2C_TIMEOUT_UserCallback(4);
	}
	
	
	I2C_GenerateSTART(I2C1,ENABLE);
	/* 检测 EV5 事件并清除标志 */
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if( (I2CTimeout--) == 0 )
			return I2C_TIMEOUT_UserCallback(5);
	}	
	
	/* 发送 EEPROM 设备地址 */
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//检测EV6事件并清除标志
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(6);		
	}
	
	/* 发送要写入的 EEPROM 内部地址 (即 EEPROM 内部存储器的地址) */
	I2C_SendData(I2C1,WriteAddr);
	I2CTimeout = I2CT_LONG_TIMEOUT;
	//检测EV8事件并清除标志
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(7);
	}	
	
	/* 循环发送 NumByteToWrite 个数据 */
	while( NumByteToWrite-- )
	{
		I2C_SendData(I2C1,*pBuf);
		pBuf++;
		I2CTimeout = I2CT_LONG_TIMEOUT;
		//检测EV8事件并清除标志
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeout--) == 0) 
				return I2C_TIMEOUT_UserCallback(8);
		}	
	}

	/* 发送停止信号 */
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
}



/*
	从EEPROM读取数据
	pBuffer: 存放从 EEPROM 读取的数据的缓冲区指针	
*/
uint8_t I2C_EE_BufRead(uint8_t* pBuf,uint8_t ReadAddr,u16 NumByteToRead)
{
	I2CTimeout = I2CT_LONG_TIMEOUT;	
	while( I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY) )
	{
		if( (I2CTimeout--) == 0 )
			return I2C_TIMEOUT_UserCallback(9);
	}
	
	
	I2C_GenerateSTART(I2C1,ENABLE);
	/* 检测 EV5 事件并清除标志 */
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if( (I2CTimeout--) == 0 )
			return I2C_TIMEOUT_UserCallback(10);
	}	
	
	/* 发送 EEPROM 设备地址 */
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//检测EV6事件并清除标志
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(11);		
	}	
	/* 通过重新设置 PE 位清除 EV6 事件 */
	I2C_Cmd(I2C1, ENABLE);
	
	/* 发送要读取的 EEPROM 内部地址 (即 EEPROM 内部存储器的地址) */
	I2C_SendData(I2C1, ReadAddr);
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(12);		
	}

	/* 产生第二次 I2C 起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(13);		
	}
	/* 发送 EEPROM 设备地址 */
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Receiver);
	//检测EV6事件并清除标志
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(14);		
	}

	while( NumByteToRead )
	{
		/* 若 NumByteToRead=1，表示已经接收到最后一个数据了，
			发送非应答信号，结束传输 */
		if(NumByteToRead == 1 )
		{
			/* 发送非应答信号 */
			I2C_AcknowledgeConfig(I2C1, DISABLE);
			/* 发送停止信号 */
			I2C_GenerateSTOP(I2C1,ENABLE);
		}
		
		I2CTimeout = I2CT_LONG_TIMEOUT;
		while( I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED) == 0 )
		{
			if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(3);			
		}
		
		{
			/* 通过I2C,从设备中读取一个字节的数据 */
			*pBuf = I2C_ReceiveData(I2C1);
			pBuf++;
			NumByteToRead -- ;
		}	
	}
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	return 1;
}


/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}



static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
	EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);
	return 0;	
}



