#include "bsp_exti.h"

/* ����Ƕ�������жϿ�����NVIC */
static void NVIC_Config(void)
{
	NVIC_InitTypeDef MVIC_InitStrcture;
	/* ����NVICΪ���ȼ���1(��01234��) ��1(1λ)-�����ȼ�0-1  ��(3bit 0-7)*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	MVIC_InitStrcture.NVIC_IRQChannel= KEY1_NVIC_IRQChannel;      //�ж�Դ
	MVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	MVIC_InitStrcture.NVIC_IRQChannelSubPriority=1;  //��/��Ӧ���ȼ�
	MVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;	// �ж�ʹ�ܻ���ʧ��
	
	NVIC_Init(&MVIC_InitStrcture);
	
	MVIC_InitStrcture.NVIC_IRQChannel= KEY2_NVIC_IRQChannel;      //�ж�Դ
	NVIC_Init(&MVIC_InitStrcture);
}	


void exti_key_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;	
	
		/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK,ENABLE);
												
	/* ���� NVIC �ж�*/
	NVIC_Config();
	
	/*KEY1*/
	/* ѡ�񰴼��õ���GPIO */	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	/* ����Ϊ�������� */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*KEY2*/
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	/* ����Ϊ�������� */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	
}


