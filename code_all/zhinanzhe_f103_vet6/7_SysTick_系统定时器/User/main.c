  // 相当于51单片机中的  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_SysTick.h"





int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 

//	SysTick_Init();
//	Delay_us(100); //100*10us=1ms
	
	
	while (1)
	{
		LED1( ON ); 
		SysTick_Delay_Us(  1*1000*1000 );
		LED1( OFF );
	  
		LED2( ON );
		SysTick_Delay_Us(  1*1000*1000 );
		LED2( OFF );
	
		LED3( ON );
		SysTick_Delay_Us(  1*1000*1000 );
		LED3( OFF );

		
	}
}


/*********************************************END OF FILE**********************/
