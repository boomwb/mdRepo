#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "oled.h"
#include "bsp_spi_flash.h"
/*
SPI1 SCK PA5
	 MIS0 PA6
	 MOSI PA7

����: MCU   oled
	  C4	CS
	  E6	DC
	  A7 	RES	
	  A5	SDA
	  A4	SCK
	  

*/
volatile u32 flash_ID = 0 ;
int main(void)
{	

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	USART_Config();
	printf(" \r\n 8M byte�ֽ� ����flashʵ�� \r\n");
	/* �жϷ�ʽ */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	
	Flash_spi_init();

	//SPI Flash ID
	flash_ID = SPI_FLASH_ReadID();
//	SysTick_Delay_Ms(200);
	
	//SPI Device ID
	//u32 DeviceID = SPI_FLASH_ReadDeviceID();
	
	printf("FlashID is 0x%X \r\n", flash_ID);
	printf("%#X \r\n ",flash_ID);
	while(1);
}


