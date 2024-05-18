#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "oled.h"
#include "sys.h"
#include <string.h>

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);


int main(void)
{	

	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	USART_Config();

	/* 中断方式需要sysTick_Init */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	/*  计数模式 不需要 */
	//	SysTick_Delay_Ms(200);
	
	I2C_EEPROM_Init();
	
	printf("3_13_liu\r\n");
	if( I2C_Test()==1 ) { LED_RED;}
	else LED_PURPLE;
	while(1)
	{
	
	}
}



uint8_t I2C_Test(void)
{
	u16 i;
	EEPROM_INFO("写入的数据");
	
	for( i=0; i<=255; i++)
	{
		I2c_Buf_Write[i] = i ;
		printf("0x%02X",I2c_Buf_Write[i]);
		if(i%16==15)
			printf("\n\r");
	}
	
	I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,256);
	
	EEPROM_INFO("写结束");
	
	EEPROM_INFO("读出的数据");
	I2C_EE_BufRead(I2c_Buf_Read,EEP_Firstpage,256);
	for(i=0; i<256; i++)
	{
		if( I2c_Buf_Read[i] != I2c_Buf_Write[i] )
		{
			printf("0x%02x",I2c_Buf_Read[i] );
			EEPROM_ERROR("错误:I2C EEPROM 写入与读出的数据不一致");
			return 0;
		}
		
	printf("0x%02X",I2c_Buf_Read[i] );
	if(i%16==15)
		printf("\n\r");
	}
	EEPROM_INFO("I2C(AT24C02) 读写测试成功");
	return 1;
}


