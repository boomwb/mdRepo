#ifndef  __BSP_SYSTICK__
#define  __BSP_SYSTICK__


#include "stm32f10x.h"                  // Device header


void SysTick_Init(void);

/*  ��һ�ַ�ʽ  */
//�ж� ȫ�ֱ���
void Delay_us(uint32_t ten_us); //10usÿ��
#define Delay_ms(x)  Delay_us(uint32_t 100*ten_us)


/*  �ڶ��ַ�ʽ  */
/*
	systick �� counter �� reload ֵ���µݼ��� 0 ��
	ʱ��CTRL �Ĵ�����λ 16:countflag ���� 1���Ҷ�ȡ��λ��ֵ���� 0���������ǿ���ʹ�������
	ѯ�ķ�����ʵ����ʱ��
*/

void SysTick_Delay_Us(uint32_t us);
void SysTick_Delay_Ms(uint32_t ms);



#endif


