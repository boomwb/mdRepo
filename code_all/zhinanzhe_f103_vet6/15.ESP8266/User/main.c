#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "sys.h"
#include  "basicTIM.h"
#include "core_delay.h"
#include "bsp_esp8266.h"


extern int time;




int main(void)
{	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//����ϵͳ�ж����ȼ�����2
	
	/* LED �˿ڳ�ʼ�� */
//	LED_GPIO_Config();	 
	
	
	//Motor_Init();
	/* �жϷ�ʽ */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms

	//SysTick_Delay_Ms(15);
	
//	basic_Tim_Config();
	
//	basic_Tim_NVIC_Config();

	USART_Config();	
	ESP8266_Init();

	printf("ESP8266 TES\n\n");
	while(1)
	{	
		if(strUSART_Fram_Record.InfBit.FramFinishFlag == 1)  //������յ��˴��ڵ������ֵ�����
		{
			strUSART_Fram_Record.Data_RX_BUF[strUSART_Fram_Record.InfBit.FramLength] = '\0';
			Usart_SendString(macESP8266_USARTx ,strUSART_Fram_Record.Data_RX_BUF);      //���ݴӴ��ڵ�������ת����ESP8266
			strUSART_Fram_Record.InfBit.FramLength = 0;                                //�������ݳ�������
			strUSART_Fram_Record.InfBit.FramFinishFlag = 0;                            //���ձ�־����
	  }
		if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)                             //������յ���ESP8266������
		{                                                      
			 strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
			 Usart_SendString(DEBUG_USARTx ,strEsp8266_Fram_Record.Data_RX_BUF);        //���ݴ�ESP8266ת�������ڵ�������
			 strEsp8266_Fram_Record.InfBit.FramLength = 0;                             //�������ݳ�������
			 strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //���ձ�־����
		}
  }	
	
//		if(time==1000)
//		{
//			//PBout(0)^=1;
//			PBout(0)=~PBout(0);
//			//LED3_TOGGLE;
//			time=0;
//		}
	
	
	
		
	
}


