#ifndef  __BSP_SYSTICK__
#define  __BSP_SYSTICK__


#include "stm32f10x.h"                  // Device header


void SysTick_Init(void);

/*  第一种方式  */
//中断 全局变量
void Delay_us(uint32_t ten_us); //10us每次
#define Delay_ms(x)  Delay_us(uint32_t 100*ten_us)


/*  第二种方式  */
/*
	systick 的 counter 从 reload 值往下递减到 0 的
	时候，CTRL 寄存器的位 16:countflag 会置 1，且读取该位的值可清 0，所有我们可以使用软件查
	询的方法来实现延时。
*/

void SysTick_Delay_Us(uint32_t us);
void SysTick_Delay_Ms(uint32_t ms);



#endif


