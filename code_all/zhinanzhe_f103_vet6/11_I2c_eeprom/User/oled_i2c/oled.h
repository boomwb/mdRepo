#ifndef  OLED_H
#define  OLED_H

#include "stm32f10x.h"
#include "bsp_usart.h"

#define  I2C_Speed  		400000 //400Kbit/s 快速模式
#define  I2Cx_OWN_ADDR 	     0X0A 

#define  I2C_PageSize      8 //每页有8个字节

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT  ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT  ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*信息输出*/
#define EEPROM_DEBUG_ON         0

#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/* 
 * AT24C02 2kb = 2048Byte = 2048/8 B = 256 Bit
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   


static void I2C_GPIO_Config(void);
static void I2C_Mpde_Config(void);
void I2C_EEPROM_Init(void);

uint32_t I2C_EE_Write_Byte(u8 *pBuf, u8 WriteAddr);
void I2C_EE_WaitEepromStandbyState(void) ;     
uint8_t I2C_EE_Bytes_write(uint8_t* pBuf,uint8_t WriteAddr,uint16_t NumByteToWrite);
uint8_t I2C_EE_PageWrite(uint8_t* pBuf, uint8_t WriteAddr,uint8_t NumByteToWrite);										  
uint8_t I2C_EE_BufRead(uint8_t* pBuf,uint8_t ReadAddr,u16 NumByteToRead);	

void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
	
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
#endif
