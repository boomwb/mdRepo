  // �൱��51��Ƭ���е�  #include <reg51.h>


// STM32F10X_HD, USE_STDPERIPH_DRIVER

	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"


//�����λ����8 
void System_Reset(void) 
{
	
    __set_FAULTMASK(1); //�ر������ж�;
    NVIC_SystemReset(); //���������λ
}



int main(void)
{	
	char chr;
	chr = 'a';
	USART_Config();
//	Usart_SendByte(USART1, 'a');
	Usart_SendString( USART1,"����ʵ��\n");
		printf("\r\n please write your name: ");
		
		
	//	scanf("%s",&chr);
		printf("\r\n Hello ,%c \r\n",chr);
	
	System_Reset();
	while (1)
	{


	}
}


/*********************************************END OF FILE**********************/
