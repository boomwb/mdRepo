#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "sys.h"
#include  "basicTIM.h"

//正转
void Motor_CW(void)
{




}

extern int time;

int main(void)
{	

	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	USART_Config();
//	Motor_Init();
	/* 中断方式 */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms

	//SysTick_Delay_Ms(15);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	basic_Tim_Config();
	
	basic_Tim_NVIC_Config();
	
	

	
	while(1)
	{
	
	
		if(time==1000)
		{
			//PBout(0)^=1;
			PBout(0)=~PBout(0);
			//LED3_TOGGLE;
			time=0;
		}
	
	
	}
		
	
}


