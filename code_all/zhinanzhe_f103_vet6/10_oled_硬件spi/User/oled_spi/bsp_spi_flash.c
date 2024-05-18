#include "bsp_spi_flash.h"
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT; 
//volatile


void Flash_spi_init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//SPI1 Clock enanle
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	//GPIO Clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	
	//gpio init 
	//cs
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin  = flash_spi_cs_pin;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(Flash_spi_cs_port,&GPIO_InitStruct);
	
	//sck
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin  = flash_spi_sck_pin;
	GPIO_Init(Flash_spi_sck_port,&GPIO_InitStruct);
	
	//miso
	GPIO_InitStruct.GPIO_Pin  = flash_spi_miso_pin;
	GPIO_Init(Flash_spi_miso_port,&GPIO_InitStruct);	
	
	//mosi
	GPIO_InitStruct.GPIO_Pin  = flash_spi_mosi_pin;
	GPIO_Init(Flash_spi_mosi_port,&GPIO_InitStruct);	
	
	//����spi_cs
	flash_spi_cs_high();
	
	//FLASH оƬ ֧�� SPI ģʽ 0 ��ģʽ 3���ݴ����� CPOL CPHA
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode		 = SPI_Mode_Master; //SPI_Mode_Master
	SPI_InitStruct.SPI_DataSize  = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL 	= SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA 	= SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS 		= SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,  &SPI_InitStruct);
	
	/* ʹ�� SPI */
	SPI_Cmd(SPI1,ENABLE);
}

/*
	spi����һ���ֽڵ�����,
	����ֵ:���ܵ�������
*/
u8 Flash_spi_sendbyte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* �ȴ����ͻ�����Ϊ�գ�TXE �¼� */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET)
	{
		if( (SPITimeout--) == 0 ) 
			return 0;
	}
	
	//д�����ݼĴ���,��Ҫд������д�뵽���ͻ�����
	SPI_I2S_SendData(SPI1,byte);
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ջ������ǿգ�RXNE �¼� -ȷ���Ѿ����ܵ�����*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET)
	{
		if( (SPITimeout--) == 0 ) 
			return 1;		
	}
	
	//��ȡ���ݼĴ���,��ȡ����������
	return SPI_I2S_ReceiveData(SPI1);
}

/*
	ʹ��SPI��ȡһ���ֽڵ�����
	����ֵ:�յ�������
*/
u8 Flash_spi_readbyte(void)
{
	//����Ϊ����ֵ������������ԭ���� SPI �Ľ��չ��̺�
	//���͹���ʵ����һ���ģ��շ�ͬ�����У��ؼ�����
	//���ǵ��ϲ�Ӧ���У���ע���Ƿ��ͻ��ǽ��յ�����
	return Flash_spi_sendbyte(Dummy_Byte);
}

u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
	//��ʼͨѶ cs��
	flash_spi_cs_low();
	
	//����ָ�� ��ȡID
	Flash_spi_sendbyte(W25X_JedecDeviceID);
	//��ȡһ���ֽ�����
	Temp0 = Flash_spi_sendbyte(Dummy_Byte);
	//��ȡһ���ֽ�����
	Temp1 = Flash_spi_sendbyte(Dummy_Byte);	
	//��ȡһ���ֽ�����
	Temp2 = Flash_spi_sendbyte(Dummy_Byte);	
	
	//endͨѶ cs��
	flash_spi_cs_high();
	
	/* �����������������Ϊ�����ķ���ֵ */
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	
	return Temp;
}

/* SPI дʹ�� */
void SPI_FLASH_WriteEnable(void)
{
	//��ʼͨѶ cs��
	flash_spi_cs_low();
	Flash_spi_sendbyte(W25X_WriteEnable);
	//endͨѶ cs��
	flash_spi_cs_high();	
}
void SPI_FLASH_WriteDisable(void)
{
	//��ʼͨѶ cs��
	flash_spi_cs_low();
	Flash_spi_sendbyte(W25X_WriteDisable);
	//endͨѶ cs��
	flash_spi_cs_high();	
}

/* �ȴ� WIP(BUSY)��־����0 BUSYλ=1 ����æ */
void SPI_FLASH_Wait_Notbusy(void)
{
	u8 FLASH_Status = 0;
	
	//��ʼͨѶ cs��
	flash_spi_cs_low();
	
	/* ���� ��״̬�Ĵ��� ���� */
	Flash_spi_sendbyte(W25X_ReadStatusReg);
	
	/* �� FLASH æµ����ȴ� */
	do
	{
		FLASH_Status = Flash_spi_sendbyte(Dummy_Byte);
	}
	while( (FLASH_Status & WIP_Flag) == SET); // &0x01 ==1 
	
	
	//endͨѶ cs��
	flash_spi_cs_high();			
}


/*
	FLASH �洢�������Ծ�������ֻ�ܰ�ԭ��Ϊ��1��������λ��д�ɡ�0��
	�Ѿ����е�����λ����Ϊ��1��
	�������� Sector Erase 4kb
	�����   Block Erase 64kb
	������� chip Erase
	128block 128*64=8192Kb
*/
void SectorErase(u32 Sector_addr)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_Wait_Notbusy();
	
	//��ʼͨѶ cs��
	flash_spi_cs_low();
	
	Flash_spi_sendbyte(W25X_SectorErase);
	//�ٷ���24λ��ַ
	Flash_spi_sendbyte((Sector_addr&0xFF0000) >> 16);
	Flash_spi_sendbyte((Sector_addr&0xFF00) >> 8);
	Flash_spi_sendbyte((Sector_addr&0xFF) >> 0);
	
	flash_spi_cs_high();	
	
	SPI_FLASH_Wait_Notbusy();
}

//ҳд��  wr_length-д�����ݳ���,����С��ҳ��С
void SPI_Flash_PageWrite(u8 *pdata, u32 wr_addr,u16 wr_length)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_Wait_Notbusy();
	
	//��ʼͨѶ cs��
	flash_spi_cs_low();
	
	Flash_spi_sendbyte(W25X_PageProgram);
	//�ٷ���24λ��ַ
	Flash_spi_sendbyte((wr_addr&0xFF0000) >> 16);
	Flash_spi_sendbyte((wr_addr&0xFF00) >> 8);
	Flash_spi_sendbyte((wr_addr&0xFF) >> 0);

	if(wr_length > SPI_FLASH_PerWritePageSize)
	{
		wr_length = SPI_FLASH_PerWritePageSize;
	}
		
	
	while(wr_length --)
	{
		Flash_spi_sendbyte(*pdata);
		pdata++;
	}
	
	flash_spi_cs_high();	
	
	SPI_FLASH_Wait_Notbusy();
}


/*
	����������д��
	wr_length д�����ݳ���
*/
void SPI_Flash_buff_write(u8 *pdata, u32 wr_addr, u16 wr_length)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	//���wr_addr��256��������Addr=0 
	Addr = wr_addr % SPI_FLASH_PageSize;
	
	//��count ������ֵ�Ϳ��Զ��뵽ҳ��ַ
	count = SPI_FLASH_PageSize - Addr;
	
	//Ҫд���ٸ�����ҳ
	NumOfPage = wr_length / SPI_FLASH_PageSize;
	
	//mod�������� �����ʣ�಻��һҳ���ֽ���
	NumOfSingle = wr_length % SPI_FLASH_PageSize;
	
	if(Addr == 0)
	{
		if(NumOfPage == 0) //д��ĳ���С��256
		{
			SPI_Flash_PageWrite(pdata, wr_addr, wr_length);
		}
		else
		{
			while(NumOfPage--)
			{
				SPI_Flash_PageWrite(pdata, wr_addr, wr_length);
				wr_addr += SPI_FLASH_PageSize;
				pdata += SPI_FLASH_PageSize;
			}
			
			/* ���ж���Ĳ���һҳ�����ݣ�����д�� */
			SPI_Flash_PageWrite(pdata, wr_addr, wr_length);
		}
	}
	
	
	/* ����ַ�� SPI_FLASH_PageSize ������ */
	else
	{
		if(NumOfPage == 0) 	 //д��<256
		{
			/* ��ǰҳʣ��� count ��λ�ñ� NumOfSingle С��һҳд���� */
			if(NumOfSingle > count) 
			{
				temp = NumOfSingle - count;
				//��д����ǰҳ
				SPI_Flash_PageWrite(pdata, wr_addr, count);
				
				wr_addr += count;
				pdata += count;

				SPI_Flash_PageWrite(pdata, wr_addr, temp);
			}
			else /* ��ǰҳʣ���count��λ����д�� Num_singleʣ�಻��һҳ���ֽ��� */
			{
				SPI_Flash_PageWrite(pdata, wr_addr, wr_length);
			}
		}
		
		//wr_length > 256 ������
		else
		{
			wr_length -= count; //��count ������ֵ�Ϳ��Զ��뵽ҳ��ַ
			
			NumOfPage = wr_length / SPI_FLASH_PageSize;
			NumOfSingle = wr_length % SPI_FLASH_PageSize;
			
			/* ��д�� count �����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
			SPI_Flash_PageWrite(pdata, wr_addr, count);
			
			wr_addr += count;
			pdata += count;
			while(NumOfPage--)
			{
				SPI_Flash_PageWrite(pdata, wr_addr, SPI_FLASH_PageSize);
				wr_addr += SPI_FLASH_PageSize;
				pdata += SPI_FLASH_PageSize;
			}
			if(NumOfSingle !=0 )
			{
				SPI_Flash_PageWrite(pdata, wr_addr, NumOfSingle);
			}
			
		}			
		
	
	}

}



