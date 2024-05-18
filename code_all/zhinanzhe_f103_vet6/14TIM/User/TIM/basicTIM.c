#include  "basicTIM.h"

//定时器 2-7  (6 7 基本定时器 2345通用)APB1 最大36M
// tim1 和 tim8--高级定时器 APB2 MAX=72M 
int time=0;

void basic_Tim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	//开启定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =TIM_CKD_DIV1; //时钟分频
	//设置定时器时钟 CK_INT 频率与数字滤波器采样时钟频率分频比，基本定时器没有此功能，不用设置。
	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //计数方式 基本定时器只能是向上计数
	
	TIM_TimeBaseInitStruct.TIM_Period =1000;	//自动重装载寄存器的值
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71; //定时器预分频器设置
	//时钟源经该预分频器才是定时器时钟，它设定 TIMx_PSC寄存器的值。可设置范围为 0 至 65535，实现 1 至 65536 分频。
	
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0Xff;
	//重复计数器，属于高级控制寄存器专用寄存器位，利用它可以非常容易控制输出 PWM 的个数
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	
	//clear 计数标志位 Clears the TIMx's pending flags.
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	
	
	//Enables or disables the specified TIM interrupts.
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//使能计算器
	TIM_Cmd(TIM6, ENABLE);
	
	 //暂时关闭定时器的时钟，等待使用
	
}


void basic_Tim_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
/*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); */
	//组0  0bit主优先级(抢占优先级)   4bit响应优先级
	//NVIC_PriorityGroup_4         4 bits for pre-emption priority 0 bits for subpriority */
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0; //主优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3; //子优先级
	NVIC_Init(&NVIC_InitStruct);
}

void TIM6_IRQHandler(void) //定时器中断一次的时间是 1ms
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		time++;
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	
	}

}
