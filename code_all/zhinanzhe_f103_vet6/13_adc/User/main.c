#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_adc.h"

#include "sys.h"

extern __IO uint16_t ADC_Value; //12λ������

float Local_Value ;


int main(void)
{	

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	

	/* �жϷ�ʽ��ҪsysTick_Init */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	/*  ����ģʽ ����Ҫ */
	//	SysTick_Delay_Ms(200);
	ADCx_Init();
	USART_Config();
	printf(" \r\n  ADCʵ�� \r\n");
	while(1)
	{
		Local_Value =(float)ADC_Value/4096*3.3;
		printf("����valueΪ0x%04x \r\n",ADC_Value);
		printf("ģ���ѹΪ:%f V \r\n",Local_Value);
		
		SysTick_Delay_Ms(10000);
	}
}


