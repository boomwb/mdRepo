  // �൱��51��Ƭ���е�  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"



#define SOFT_DELAY Delay(0x0FFFFF);




int main(void)
{	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	/* ��ʼ��EXTI�жϣ����°����ᴥ���жϣ�
	*  �����жϻ����stm32f10x_it.c�ļ��еĺ���
	�������ļ� startup_stm32f10x_hd.s ������Ԥ��Ϊÿ���ж϶�д��һ���жϷ�������ֻ����Щ
	�жϺ�������Ϊ�գ�Ϊ��ֻ�ǳ�ʼ���ж�������ʵ�ʵ��жϷ���������Ҫ�������±�д��Ϊ
	�˷���������ǰ��жϷ�����ͳһд�� stm32f10x_it.c ������ļ��С�
	*  KEY1_IRQHandler��KEY2_IRQHandler�������жϣ���תLED�ơ�
	*/
	exti_key_config();
	
	while (1)
	{
	

		
	}
}


/*********************************************END OF FILE**********************/
