#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "sys.h"
#include  "basicTIM.h"

//��ת
void Motor_CW(void)
{




}

extern int time;

int main(void)
{	

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	USART_Config();
//	Motor_Init();
	/* �жϷ�ʽ */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms

	//SysTick_Delay_Ms(15);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2

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


