 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./flash/bsp_spi_flash.h"



/**
  * @brief  SPI_FLASH初始化
  * @param  PC0---CE  A5--SCK A6-SDK
  
  * @retval 无
  */
//void SPI_FLASH_Init(void)
//{
//  SPI_InitTypeDef  SPI_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/* 使能SPI时钟 */
//	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );
//	
//	/* 使能SPI引脚相关的时钟 */
// 	FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|FLASH_SPI_MISO_PIN, ENABLE );
//	
//  /* 配置SPI的 CS引脚，普通IO即可 */
//  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);
//	
//  /* 配置SPI的 SCK引脚*/
//  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

//  /* 配置SPI的 MISO引脚*/
//  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
//  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

////  /* 配置SPI的 MOSI引脚*/
////  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
////  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

//  /* 停止信号 FLASH: CS引脚高电平*/
//  SPI_FLASH_CS_HIGH();

//  /* SPI 模式配置 */
//  // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  SPI_Init(FLASH_SPIx , &SPI_InitStructure);

//  /* 使能 SPI  */
//  SPI_Cmd(FLASH_SPIx , ENABLE);
//	
//}
// /**
//  * @brief  擦除FLASH扇区
//  * @param  SectorAddr：要擦除的扇区地址
//  * @retval 无
//  */


void ds1302_gpio_init()//CE,SCLK?????
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PC.0  CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOC.11
	GPIO_ResetBits(GPIOC,GPIO_Pin_0); 
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //Pa 5   SCLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOC.12
	GPIO_ResetBits(GPIOA,GPIO_Pin_5); 
}
 
void ds1032_DATAOUT_init()//PA6
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //Pa 5   SCLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOC.12
	GPIO_ResetBits(GPIOA,GPIO_Pin_6); 
}
 
void ds1032_DATAINPUT_init()//????I/O??????
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PC.10 DATA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOC.10
}
 
 
void ds1302_write_onebyte(u8 data)//?DS1302???????
{
ds1032_DATAOUT_init();
u8 count=0;
SCLK_L;
for(count=0;count<8;count++)
	{	SCLK_L;
		if(data&0x01)
		{DATA_H;}
		else{DATA_L;}//?????????
		SCLK_H;//?????,????
		data>>=1;
	}
}
 
void ds1302_wirte_rig(u8 address,u8 data)//????????????
{
u8 temp1=address;
u8 temp2=data;
CE_L;SCLK_L;delay_us(1);
CE_H;delay_us(2);
ds1302_write_onebyte(temp1);
ds1302_write_onebyte(temp2);
CE_L;SCLK_L;delay_us(2);
}
 
u8 ds1302_read_rig(u8 address)//????????????
{
u8 temp3=address;
u8 count=0;
u8 return_data=0x00;
CE_L;SCLK_L;delay_us(3);
CE_H;delay_us(3);
ds1302_write_onebyte(temp3);
ds1032_DATAINPUT_init();//??I/O????
delay_us(2);
for(count=0;count<8;count++)
	{
	delay_us(2);//?????????
	return_data>>=1;
	SCLK_H;delay_us(4);//??????????
	SCLK_L;delay_us(14);//??14us???????,????
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10))
	{return_data=return_data|0x80;}
	
	}
delay_us(2);
CE_L;DATA_L;
return return_data;
}
 
void ds1032_init()
{
ds1302_wirte_rig(0x8e,0x00);//?????
ds1302_wirte_rig(0x80,0x37);//seconds37?
ds1302_wirte_rig(0x82,0x58);//minutes58?
ds1302_wirte_rig(0x84,0x23);//hours23?
ds1302_wirte_rig(0x86,0x30);//date30?
ds1302_wirte_rig(0x88,0x09);//months9?
ds1302_wirte_rig(0x8a,0x07);//days???
ds1302_wirte_rig(0x8c,0x20);//year2020?
ds1302_wirte_rig(0x8e,0x80);//?????
}
 
void ds1032_read_time()
{
read_time[0]=ds1302_read_rig(0x81);//??
read_time[1]=ds1302_read_rig(0x83);//??
read_time[2]=ds1302_read_rig(0x85);//??
read_time[3]=ds1302_read_rig(0x87);//??
read_time[4]=ds1302_read_rig(0x89);//??
read_time[5]=ds1302_read_rig(0x8B);//???
read_time[6]=ds1302_read_rig(0x8D);//??
}
 
void ds1032_read_realTime()
{
ds1032_read_time();  //BCD????10??
TimeData.second=(read_time[0]>>4)*10+(read_time[0]&0x0f);
TimeData.minute=((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
TimeData.hour=(read_time[2]>>4)*10+(read_time[2]&0x0f);
TimeData.day=(read_time[3]>>4)*10+(read_time[3]&0x0f);
TimeData.month=(read_time[4]>>4)*10+(read_time[4]&0x0f);
TimeData.week=read_time[5];
TimeData.year=(read_time[6]>>4)*10+(read_time[6]&0x0f)+2000;
}

 