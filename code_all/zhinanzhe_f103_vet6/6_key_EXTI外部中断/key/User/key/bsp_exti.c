#include "bsp_exti.h"

/* 配置嵌套向量中断控制器NVIC */
static void NVIC_Config(void)
{
	NVIC_InitTypeDef MVIC_InitStrcture;
	/* 配置NVIC为优先级组1(共01234组) 组1(1位)-主优先级0-1  子(3bit 0-7)*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	MVIC_InitStrcture.NVIC_IRQChannel= KEY1_NVIC_IRQChannel;      //中断源
	MVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	MVIC_InitStrcture.NVIC_IRQChannelSubPriority=1;  //子/响应优先级
	MVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;	// 中断使能或者失能
	
	NVIC_Init(&MVIC_InitStrcture);
	
	MVIC_InitStrcture.NVIC_IRQChannel= KEY2_NVIC_IRQChannel;      //中断源
	NVIC_Init(&MVIC_InitStrcture);
}	


void exti_key_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;	
	
		/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK,ENABLE);
												
	/* 配置 NVIC 中断*/
	NVIC_Config();
	
	/*KEY1*/
	/* 选择按键用到的GPIO */	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	/* 配置为浮空输入 */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	/* 选择EXTI的信号源 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*KEY2*/
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	/* 配置为浮空输入 */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	/* 选择EXTI的信号源 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	
}


