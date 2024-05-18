#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"



int main(void)
{	

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	USART_Config();
	
	/* �жϷ�ʽ */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2



	
	while(1) 
	{		
		
		SysTick_Delay_Ms(500);
		//LED3_ON;
	}
}


