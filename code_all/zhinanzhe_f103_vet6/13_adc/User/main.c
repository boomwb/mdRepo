#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_adc.h"

#include "sys.h"

extern __IO uint16_t ADC_Value; //12位的数字

float Local_Value ;


int main(void)
{	

	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	

	/* 中断方式需要sysTick_Init */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	/*  计数模式 不需要 */
	//	SysTick_Delay_Ms(200);
	ADCx_Init();
	USART_Config();
	printf(" \r\n  ADC实验 \r\n");
	while(1)
	{
		Local_Value =(float)ADC_Value/4096*3.3;
		printf("数字value为0x%04x \r\n",ADC_Value);
		printf("模拟电压为:%f V \r\n",Local_Value);
		
		SysTick_Delay_Ms(10000);
	}
}


