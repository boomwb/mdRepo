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

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	USART_Config();

	/* �жϷ�ʽ��ҪsysTick_Init */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	/*  ����ģʽ ����Ҫ */
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
	EEPROM_INFO("д�������");
	
	for( i=0; i<=255; i++)
	{
		I2c_Buf_Write[i] = i ;
		printf("0x%02X",I2c_Buf_Write[i]);
		if(i%16==15)
			printf("\n\r");
	}
	
	I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,256);
	
	EEPROM_INFO("д����");
	
	EEPROM_INFO("����������");
	I2C_EE_BufRead(I2c_Buf_Read,EEP_Firstpage,256);
	for(i=0; i<256; i++)
	{
		if( I2c_Buf_Read[i] != I2c_Buf_Write[i] )
		{
			printf("0x%02x",I2c_Buf_Read[i] );
			EEPROM_ERROR("����:I2C EEPROM д������������ݲ�һ��");
			return 0;
		}
		
	printf("0x%02X",I2c_Buf_Read[i] );
	if(i%16==15)
		printf("\n\r");
	}
	EEPROM_INFO("I2C(AT24C02) ��д���Գɹ�");
	return 1;
}


