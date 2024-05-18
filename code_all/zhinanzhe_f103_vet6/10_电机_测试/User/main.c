#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "oled.h"
#include "sys.h"

int main(void)
{	
	u8 t=0;
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();

	/* ��� IO ��ʼ�� */
	Tianji_GPIO_Config();
	USART_Config();
	
	/* �жϷ�ʽ */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1m
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	


	while(1) 
	{		
		PAout(5)=1;
		PAout(7)=0;
		PEout(6)=1;
		PCout(4)=0;
		
		SysTick_Delay_Ms(3);
		PAout(5)=0;
		PAout(7)=1;
		PEout(6)=1;
		PCout(4)=0;		

		SysTick_Delay_Ms(3);

		PAout(5)=0;
		PAout(7)=1;
		PEout(6)=0;
		PCout(4)=1;		
		SysTick_Delay_Ms(3);

		PAout(5)=1;
		PAout(7)=0;
		PEout(6)=0;
		PCout(4)=1;	
		SysTick_Delay_Ms(3);
	}
}


