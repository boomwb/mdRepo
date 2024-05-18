  // 相当于51单片机中的  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

	
#include "stm32f10x.h"
#include "bsp_led.h"

#define SOFT_DELAY Delay(0x0FFFFF);




int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 

	while (1)
	{
		LED3_TOGGLE;
		

		//寄存器
//		GPIOB->BSRR=GPIO_Pin_5;
//		GPIOB->BRR =GPIO_Pin_1;
//		GPIOB->BRR =GPIO_Pin_0;
		
	}
}


/*********************************************END OF FILE**********************/
