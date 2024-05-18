#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

/*
SPI1 APB2  MAX=36M bit/s
SPI2/3 APB1  18M

SPI1 SCK PA5
	 MIS0 PA6
	 MOSI PA7
*/
#include "stm32f10x.h"
#include <stdio.h>
#include "sys.h"

#define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*命令定义-开头*******************************/
#define W25X_WriteEnable		        0x06 
#define W25X_WriteDisable		      	0x04 
#define W25X_ReadStatusReg		    	0x05 
#define W25X_WriteStatusReg		    	0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		     	0x0B 
#define W25X_FastReadDual		     	0x3B //双的
#define W25X_PageProgram		     	0x02 
#define W25X_BlockErase			     	0xD8 
#define W25X_SectorErase		     	0x20 
#define W25X_ChipErase			     	0xC7 
#define W25X_PowerDown			     	0xB9 
#define W25X_ReleasePowerDown	    	0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   		0x90 
#define W25X_JedecDeviceID		    	0x9F

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte 0xFF


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))



//CS(NSS) 普通GPIO 软件控制
#define Flash_spi_cs_port GPIOC
#define flash_spi_cs_pin 	GPIO_Pin_0

//SCK
#define Flash_spi_sck_port GPIOA
#define flash_spi_sck_pin 	GPIO_Pin_5

//MISO
#define Flash_spi_miso_port GPIOA
#define flash_spi_miso_pin 	GPIO_Pin_6

//MOSI
#define Flash_spi_mosi_port GPIOA
#define flash_spi_mosi_pin 	GPIO_Pin_7


//函数
#define flash_spi_cs_low()  	PCout(0)=0
#define flash_spi_cs_high()  	PCout(0)=1



void Flash_spi_init(void);
u8 Flash_spi_sendbyte(u8 byte);
u8 Flash_spi_readbyte(void);
u32 SPI_FLASH_ReadID(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
void SPI_FLASH_Wait_Notbusy(void);
void SectorErase(u32 Sector_addr);



#endif
