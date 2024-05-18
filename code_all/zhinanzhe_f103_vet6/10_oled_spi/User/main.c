#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "oled.h"


int main(void)
{	
	u8 t=0;
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	USART_Config();
	
	/* 中断方式 */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	

	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2


 	OLED_Init();				//初始化OLED
	OLED_ShowString(0,0,"EEEELLO",24);  
	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,40,"ATOM 2024/12/31",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12);  
	OLED_Refresh_Gram();//更新显示到OLED	 
	t=' ';  
	while(1) 
	{		
		OLED_ShowChar(36,52,t,12,1);//显示ASCII字符	
		OLED_ShowNum(94,52,t,3,12);	//显示ASCII字符的码值    
		OLED_Refresh_Gram();        //更新显示到OLED
		t++;
		if(t>'~')t=' ';  
		SysTick_Delay_Ms(500);
		//LED3_ON;
	}
}


