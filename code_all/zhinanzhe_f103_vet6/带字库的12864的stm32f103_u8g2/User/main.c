#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

#include "bsp_u8g2.h"
//#include "mui.h"
//#include "mui_u8g2.h"
#include "u8g2.h"
//#include "font.h"


u8g2_t u8g2;

//mui_t ui;

int main(void)
{	

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	USART_Config();
	LCD12864_GPIO_Config();
	/* �жϷ�ʽ */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	/* ������ʽ */
	//SysTick_Delay_Ms
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

	u8g2_init(&u8g2);
	while(1) 
	{		

	}
}


