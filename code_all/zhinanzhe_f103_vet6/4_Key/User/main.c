#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

#include "bsp_key.h"


int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	Key_GPIO_Config();
	while(1)
	{
//		Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON
	}
}


