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
	
	//拉高spi_cs
	flash_spi_cs_high();
	
	//FLASH 芯片 支持 SPI 模式 0 及模式 3，据此设置 CPOL CPHA
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
	
	/* 使能 SPI */
	SPI_Cmd(SPI1,ENABLE);
}

/*
	spi发送一个字节的数据,
	返回值:接受到的数据
*/
u8 Flash_spi_sendbyte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 等待发送缓冲区为空，TXE 事件 */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET)
	{
		if( (SPITimeout--) == 0 ) 
			return 0;
	}
	
	//写入数据寄存器,把要写的数据写入到发送缓冲区
	SPI_I2S_SendData(SPI1,byte);
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* 等待接收缓冲区非空，RXNE 事件 -确保已经接受到数据*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET)
	{
		if( (SPITimeout--) == 0 ) 
			return 1;		
	}
	
	//读取数据寄存器,获取缓冲区数据
	return SPI_I2S_ReceiveData(SPI1);
}

/*
	使用SPI读取一个字节的数据
	返回值:收到的数据
*/
u8 Flash_spi_readbyte(void)
{
	//可以为任意值可以这样做的原因是 SPI 的接收过程和
	//发送过程实质是一样的，收发同步进行，关键在于
	//我们的上层应用中，关注的是发送还是接收的数据
	return Flash_spi_sendbyte(Dummy_Byte);
}

u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
	//开始通讯 cs低
	flash_spi_cs_low();
	
	//发送指令 读取ID
	Flash_spi_sendbyte(W25X_JedecDeviceID);
	//读取一个字节数据
	Temp0 = Flash_spi_sendbyte(Dummy_Byte);
	//读取一个字节数据
	Temp1 = Flash_spi_sendbyte(Dummy_Byte);	
	//读取一个字节数据
	Temp2 = Flash_spi_sendbyte(Dummy_Byte);	
	
	//end通讯 cs高
	flash_spi_cs_high();
	
	/* 把数据组合起来，作为函数的返回值 */
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	
	return Temp;
}

/* SPI 写使能 */
void SPI_FLASH_WriteEnable(void)
{
	//开始通讯 cs低
	flash_spi_cs_low();
	Flash_spi_sendbyte(W25X_WriteEnable);
	//end通讯 cs高
	flash_spi_cs_high();	
}
void SPI_FLASH_WriteDisable(void)
{
	//开始通讯 cs低
	flash_spi_cs_low();
	Flash_spi_sendbyte(W25X_WriteDisable);
	//end通讯 cs高
	flash_spi_cs_high();	
}

/* 等待 WIP(BUSY)标志被置0 BUSY位=1 正在忙 */
void SPI_FLASH_Wait_Notbusy(void)
{
	u8 FLASH_Status = 0;
	
	//开始通讯 cs低
	flash_spi_cs_low();
	
	/* 发送 读状态寄存器 命令 */
	Flash_spi_sendbyte(W25X_ReadStatusReg);
	
	/* 若 FLASH 忙碌，则等待 */
	do
	{
		FLASH_Status = Flash_spi_sendbyte(Dummy_Byte);
	}
	while( (FLASH_Status & WIP_Flag) == SET); // &0x01 ==1 
	
	
	//end通讯 cs高
	flash_spi_cs_high();			
}


/*
	FLASH 存储器的特性决定了它只能把原来为“1”的数据位改写成“0”
	把矩阵中的数据位擦除为“1”
	扇区擦除 Sector Erase 4kb
	块擦除   Block Erase 64kb
	整块擦除 chip Erase
	128block 128*64=8192Kb
*/
void SectorErase(u32 Sector_addr)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_Wait_Notbusy();
	
	//开始通讯 cs低
	flash_spi_cs_low();
	
	Flash_spi_sendbyte(W25X_SectorErase);
	//再发送24位地址
	Flash_spi_sendbyte((Sector_addr&0xFF0000) >> 16);
	Flash_spi_sendbyte((Sector_addr&0xFF00) >> 8);
	Flash_spi_sendbyte((Sector_addr&0xFF) >> 0);
	
	flash_spi_cs_high();	
	
	SPI_FLASH_Wait_Notbusy();
}

//页写入  wr_length-写入数据长度,必须小于页大小
void SPI_Flash_PageWrite(u8 *pdata, u32 wr_addr,u16 wr_length)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_Wait_Notbusy();
	
	//开始通讯 cs低
	flash_spi_cs_low();
	
	Flash_spi_sendbyte(W25X_PageProgram);
	//再发送24位地址
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
	不定量数据写入
	wr_length 写入数据长度
*/
void SPI_Flash_buff_write(u8 *pdata, u32 wr_addr, u16 wr_length)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	//如果wr_addr是256的正数倍Addr=0 
	Addr = wr_addr % SPI_FLASH_PageSize;
	
	//差count 个数据值就可以对齐到页地址
	count = SPI_FLASH_PageSize - Addr;
	
	//要写多少个整数页
	NumOfPage = wr_length / SPI_FLASH_PageSize;
	
	//mod运算求余 计算出剩余不满一页的字节数
	NumOfSingle = wr_length % SPI_FLASH_PageSize;
	
	if(Addr == 0)
	{
		if(NumOfPage == 0) //写入的长度小于256
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
			
			/* 若有多余的不满一页的数据，把它写完 */
			SPI_Flash_PageWrite(pdata, wr_addr, wr_length);
		}
	}
	
	
	/* 若地址与 SPI_FLASH_PageSize 不对齐 */
	else
	{
		if(NumOfPage == 0) 	 //写的<256
		{
			/* 当前页剩余的 count 个位置比 NumOfSingle 小，一页写不完 */
			if(NumOfSingle > count) 
			{
				temp = NumOfSingle - count;
				//先写满当前页
				SPI_Flash_PageWrite(pdata, wr_addr, count);
				
				wr_addr += count;
				pdata += count;

				SPI_Flash_PageWrite(pdata, wr_addr, temp);
			}
			else /* 当前页剩余的count个位置能写下 Num_single剩余不满一页的字节数 */
			{
				SPI_Flash_PageWrite(pdata, wr_addr, wr_length);
			}
		}
		
		//wr_length > 256 有余数
		else
		{
			wr_length -= count; //差count 个数据值就可以对齐到页地址
			
			NumOfPage = wr_length / SPI_FLASH_PageSize;
			NumOfSingle = wr_length % SPI_FLASH_PageSize;
			
			/* 先写完 count 个数据，为的是让下一次要写的地址对齐 */
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



