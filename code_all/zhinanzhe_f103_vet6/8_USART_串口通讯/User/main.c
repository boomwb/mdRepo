  // 相当于51单片机中的  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"


//软件复位函数8 
void System_Reset(void) 
{
	
    __set_FAULTMASK(1); //关闭所有中断;
    NVIC_SystemReset(); //进行软件复位
}



int main(void)
{	
	char chr;
	chr = 'a';
	USART_Config();
//	Usart_SendByte(USART1, 'a');
	Usart_SendString( USART1,"回显实验\n");
		printf("\r\n please write your name: ");
		
		
	//	scanf("%s",&chr);
		printf("\r\n Hello ,%c \r\n",chr);
	
	System_Reset();
	while (1)
	{


	}
}


/*********************************************END OF FILE**********************/
