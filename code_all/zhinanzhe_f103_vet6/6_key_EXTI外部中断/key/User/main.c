  // 相当于51单片机中的  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"



#define SOFT_DELAY Delay(0x0FFFFF);




int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	/* 初始化EXTI中断，按下按键会触发中断，
	*  触发中断会进入stm32f10x_it.c文件中的函数
	在启动文件 startup_stm32f10x_hd.s 中我们预先为每个中断都写了一个中断服务函数，只是这些
	中断函数都是为空，为的只是初始化中断向量表。实际的中断服务函数都需要我们重新编写，为
	了方便管理我们把中断服务函数统一写在 stm32f10x_it.c 这个库文件中。
	*  KEY1_IRQHandler和KEY2_IRQHandler，处理中断，反转LED灯。
	*/
	exti_key_config();
	
	while (1)
	{
	

		
	}
}


/*********************************************END OF FILE**********************/
