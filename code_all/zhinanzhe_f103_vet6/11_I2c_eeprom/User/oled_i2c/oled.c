#include "oled.h"

//EEPROM ��һ�ֵ�������ݲ���ʧ�Ĵ洢��
//�ͺţ�AT24C02 ��SCL--PB6   SDA--PB7  IIC1
//address ��4λ1010b ����λA0/a1/a2 
			// 1010000 0x50
//��д����λ 0 д 1010 0000 0xA0
//			 1 �� 1010 0001 0xA1

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
	I2C_InitStruct.I2C_DutyCycle	= I2C_DutyCycle_2; //ռ�ձ�
	I2C_InitStruct.I2C_ClockSpeed	= I2C_Speed;
	I2C_InitStruct.I2C_Ack			= I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C1,&I2C_InitStruct);
	
	//ʹ��I2C
	I2C_Cmd(I2C1, ENABLE);
}


void I2C_EEPROM_Init(void)
{
	I2C_GPIO_Config();
	I2C_Mpde_Config();
	EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
}

/* ��EEPROMд��һ���ֽڵ����� */
uint32_t I2C_EE_Write_Byte(u8 *pBuf, u8 WriteAddr)
{
	//* ���� I2C ��ʼ�ź� */
	I2C_GenerateSTART(I2C1,ENABLE);
	//���ó�ʱ�ȴ�ʱ��
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV5�¼��������־
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(0);
	}
	
	
	//����EEPROM�豸��ַ
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//���EV6�¼��������־
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(1);		
	}
	
	
	/* ����Ҫд��� EEPROM �ڲ���ַ (�� EEPROM �ڲ��洢���ĵ�ַ) 
			�������ָ I2C �����豸�Ķ�����ַ	    
		Ҳ�����Ϊ EEPROM �ڴ�ĵ�ַ�� I2C �豸�ļĴ�����ַ��  */
	I2C_SendData(I2C1, WriteAddr);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV8�¼��������־
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(2);
	}	
	
	
	//Sends a data byte through the I2Cx peripheral.
	I2C_SendData(I2C1,*pBuf);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//���EV8�¼��������־
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(3);
	}	

	I2C_GenerateSTOP(I2C1,ENABLE);
	
	return 1;
}

/**
  * @brief  Wait for EEPROM Standby state (����)
  * @param  ��
  * @retval ��
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

	// SR1 λ 1 ADDR��1 ��ʾ��ַ���ͳɹ���0 ��ʾ��ַ����û�н���
	// �ȴ���ַ���ͳɹ�
	while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

	/* Clear AF flag */
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	/* STOP condition */    
    I2C_GenerateSTOP(I2C1, ENABLE); 
}

/**
	���������е�����д�� I2C EEPROM �У����õ��ֽ�д��ķ�ʽ�ٶȱ�ҳд����
	NumByteToWrite: д���ֽ���
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


/* EEPROM��ҳд�� 
�����ϵ�����ͨѶ�У�ÿд��һ�����ݶ���Ҫ�� EEPROM ����д��ĵ�ַ������ϣ����������
ַд�������ݵ�ʱ��ֻҪ���� EEPROM ��һ���ڴ��ַ address1��
��������ݰ�����д�뵽address2��address3?�������Խ�ʡͨѶ��ʱ�䣬�ӿ��ٶȡ�
ΪӦ����������EEPROM ������һ��ҳд��ʱ��..

	AT24C02 �ͺŵ�оƬҳд��ʱ��������һ�η��� 8 ������
	(�� n = 8 )����ֵҲ��Ϊҳ��С
	NumByteToWrite: Ҫд���ֽ���Ҫ�� NumByToWrite С��ҳ��С
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
	/* ��� EV5 �¼��������־ */
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if( (I2CTimeout--) == 0 )
			return I2C_TIMEOUT_UserCallback(5);
	}	
	
	/* ���� EEPROM �豸��ַ */
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//���EV6�¼��������־
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(6);		
	}
	
	/* ����Ҫд��� EEPROM �ڲ���ַ (�� EEPROM �ڲ��洢���ĵ�ַ) */
	I2C_SendData(I2C1,WriteAddr);
	I2CTimeout = I2CT_LONG_TIMEOUT;
	//���EV8�¼��������־
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(7);
	}	
	
	/* ѭ������ NumByteToWrite ������ */
	while( NumByteToWrite-- )
	{
		I2C_SendData(I2C1,*pBuf);
		pBuf++;
		I2CTimeout = I2CT_LONG_TIMEOUT;
		//���EV8�¼��������־
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeout--) == 0) 
				return I2C_TIMEOUT_UserCallback(8);
		}	
	}

	/* ����ֹͣ�ź� */
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
}



/*
	��EEPROM��ȡ����
	pBuffer: ��Ŵ� EEPROM ��ȡ�����ݵĻ�����ָ��	
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
	/* ��� EV5 �¼��������־ */
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if( (I2CTimeout--) == 0 )
			return I2C_TIMEOUT_UserCallback(10);
	}	
	
	/* ���� EEPROM �豸��ַ */
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//���EV6�¼��������־
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(11);		
	}	
	/* ͨ���������� PE λ��� EV6 �¼� */
	I2C_Cmd(I2C1, ENABLE);
	
	/* ����Ҫ��ȡ�� EEPROM �ڲ���ַ (�� EEPROM �ڲ��洢���ĵ�ַ) */
	I2C_SendData(I2C1, ReadAddr);
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(12);		
	}

	/* �����ڶ��� I2C ��ʼ�ź� */
	I2C_GenerateSTART(I2C1,ENABLE);
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(13);		
	}
	/* ���� EEPROM �豸��ַ */
	I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS,I2C_Direction_Receiver);
	//���EV6�¼��������־
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) )
	{
		if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(14);		
	}

	while( NumByteToRead )
	{
		/* �� NumByteToRead=1����ʾ�Ѿ����յ����һ�������ˣ�
			���ͷ�Ӧ���źţ��������� */
		if(NumByteToRead == 1 )
		{
			/* ���ͷ�Ӧ���ź� */
			I2C_AcknowledgeConfig(I2C1, DISABLE);
			/* ����ֹͣ�ź� */
			I2C_GenerateSTOP(I2C1,ENABLE);
		}
		
		I2CTimeout = I2CT_LONG_TIMEOUT;
		while( I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED) == 0 )
		{
			if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(3);			
		}
		
		{
			/* ͨ��I2C,���豸�ж�ȡһ���ֽڵ����� */
			*pBuf = I2C_ReceiveData(I2C1);
			pBuf++;
			NumByteToRead -- ;
		}	
	}
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	return 1;
}


/**
  * @brief   ���������е�����д��I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
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
	EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);
	return 0;	
}



