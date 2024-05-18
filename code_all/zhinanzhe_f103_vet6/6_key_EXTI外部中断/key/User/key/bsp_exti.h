#ifndef __KEY_H__
#define __KEY_H__

/*
	KEY1 A0
	key2 c13
*/

#include "stm32f10x.h"                  // Device header

#define KEY1_GPIO_PORT 		  GPIOA
#define KEY1_GPIO_PIN 		  GPIO_Pin_0
#define KEY1_NVIC_IRQChannel  EXTI0_IRQn
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)

#define KEY2_GPIO_PORT 		  GPIOC
#define KEY2_GPIO_PIN 		  GPIO_Pin_13
#define KEY2_NVIC_IRQChannel  EXTI15_10_IRQn

#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)

#define Key1_Handler   EXTI0_IRQHandler
#define Key2_IRQHandler   EXTI15_10_IRQHandler

void exti_key_config(void);


#endif


