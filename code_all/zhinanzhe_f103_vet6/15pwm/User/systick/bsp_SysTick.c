#include  "bsp_SysTick.h"

#include "core_cm3.h"
#include "misc.h"

static __IO u32 TimingDelay;



void SysTick_Init(void)
{
	/* 10us �ж�һ��*/
	if( SysTick_Config(SystemCoreClock / 100000) )
	{
		while(1);
	}
}



void Delay_us(uint32_t ten_us)
{
	TimingDelay=ten_us;
	//ʹ�ܵδ�ʱ��
	SysTick->CTRL |= 1<<0;
	//�� TimingDelay Ϊ 0 ��ʱ���ʾ��ʱʱ�䵽������
		//TimingDelay ���жϺ����еݼ�
	while(TimingDelay !=0);
}

void TimingDelay_Decrement(void)
{
	if(TimingDelay !=0)
	{
		TimingDelay--;
	}

}

void SysTick_Delay_Us(uint32_t us)
{
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);
	for(i=0;i<us;i++)
	{
		while( ((SysTick->CTRL)&(1<<16))==0 );
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
	}
	
	// �ر�SysTick��ʱ��
	SysTick->CTRL &= ~(1<<0);
}
void SysTick_Delay_Ms(uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1
		// ����1ʱ����ȡ��λ����0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;	

}
