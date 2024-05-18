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
	//设置系统中断优先级分组2
	
	/* LED 端口初始化 */
//	LED_GPIO_Config();	 
	
	
	//Motor_Init();
	/* 中断方式 */
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
		if(strUSART_Fram_Record.InfBit.FramFinishFlag == 1)  //如果接收到了串口调试助手的数据
		{
			strUSART_Fram_Record.Data_RX_BUF[strUSART_Fram_Record.InfBit.FramLength] = '\0';
			Usart_SendString(macESP8266_USARTx ,strUSART_Fram_Record.Data_RX_BUF);      //数据从串口调试助手转发到ESP8266
			strUSART_Fram_Record.InfBit.FramLength = 0;                                //接收数据长度置零
			strUSART_Fram_Record.InfBit.FramFinishFlag = 0;                            //接收标志置零
	  }
		if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)                             //如果接收到了ESP8266的数据
		{                                                      
			 strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
			 Usart_SendString(DEBUG_USARTx ,strEsp8266_Fram_Record.Data_RX_BUF);        //数据从ESP8266转发到串口调试助手
			 strEsp8266_Fram_Record.InfBit.FramLength = 0;                             //接收数据长度置零
			 strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //接收标志置零
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


