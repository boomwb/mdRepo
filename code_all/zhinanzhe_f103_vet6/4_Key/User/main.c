#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

#include "bsp_key.h"


int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
	Key_GPIO_Config();
	while(1)
	{
//		Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON
	}
}


