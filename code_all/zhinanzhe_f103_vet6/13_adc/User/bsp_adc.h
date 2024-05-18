#ifndef __BSP_ADC_H__
#define	__BSP_ADC_H__


#include "stm32f10x.h"


//ADC选择
#define ADC_APBxClockFun  RCC_APB2PeriphClockCmd
#define ADCx	ADC2
#define ADC_CLK RCC_APB2Periph_ADC2

//ADC GPIO宏定义
// 注意：用作 ADC 采集的 IO 必须没有复用，否则采集电压会有影响
#define ADC_GPIO_Clock_Fun  RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK 	RCC_APB2Periph_GPIOC
#define ADC_PORT 	GPIOC
#define ADC_PIN 	GPIO_Pin_1

//ADC通道
#define ADC_CHANNEL ADC_Channel_11

// ADC中断
#define ADC_IRQ  ADC1_2_IRQn
#define ADC_IRQHandler ADC1_2_IRQHandler

static void ADCx_GPIO_Config(void);
static void ADCx_Mode_Config(void);
static void ADC_NVIC_Config(void);

void ADCx_Init(void);

#endif