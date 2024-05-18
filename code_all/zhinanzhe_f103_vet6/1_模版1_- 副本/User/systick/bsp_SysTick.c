#include  "bsp_SysTick.h"

#include "core_cm3.h"
#include "misc.h"

static __IO u32 TimingDelay;



void SysTick_Init(void)
{
	/* 10us 中断一次*/
	if( SysTick_Config(SystemCoreClock / 100000) )
	{
		while(1);
	}
}



void Delay_us(uint32_t ten_us)
{
	TimingDelay=ten_us;
	//使能滴答定时器
	SysTick->CTRL |= 1<<0;
	//当 TimingDelay 为 0 的时候表示延时时间到。变量
		//TimingDelay 在中断函数中递减
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
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
	}
	
	// 关闭SysTick定时器
	SysTick->CTRL &= ~(1<<0);
}
void SysTick_Delay_Ms(uint32_t ms)
{
	uint32_t i;	
	SysTick_Config(SystemCoreClock/1000);
	
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;	

}
