#ifndef 	__BSP_U8G2_H__ 
#define  	__BSP_U8G2_H__

#include "u8g2.h"
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "mui.h"
#include "mui_u8g2.h"


#define lcd_rw_PIN			 (GPIO_Pin_8)  //lcd_rw就是LCD的wr,有些图纸上写“WR”
#define lcd_e_PIN	         (GPIO_Pin_9)     //lcd_e就是LCD的rd
#define lcd_rs_PIN	         (GPIO_Pin_3)   //lcd_rs就是LCD的rs,有些图纸上写“A0”\“DC”\"DC0"，都是它
#define lcd_cs1_PIN	         (GPIO_Pin_5)     //lcd_cs1就是LCD的cs1
#define lcd_reset_PIN	     (GPIO_Pin_2) //lcd_reset就是LCD的reset


#define lcd_rw_set    	{GPIO_SetBits(GPIOC,lcd_rw_PIN);}
#define lcd_rw_reset 	{GPIO_ResetBits(GPIOC,lcd_rw_PIN);}

#define lcd_e_set    	{GPIO_SetBits(GPIOC,lcd_e_PIN);}
#define lcd_e_reset     {GPIO_ResetBits(GPIOC,lcd_e_PIN);}

#define lcd_rs_set    	{GPIO_SetBits(GPIOA,lcd_rs_PIN);}
#define lcd_rs_reset  	{GPIO_ResetBits(GPIOA,lcd_rs_PIN);}

#define lcd_cs1_set    	{GPIO_SetBits(GPIOB,lcd_cs1_PIN);}  //PB9
#define lcd_cs1_reset 	{GPIO_ResetBits(GPIOB,lcd_cs1_PIN);}

#define lcd_reset_set   {GPIO_SetBits(GPIOA,lcd_reset_PIN);}
#define lcd_reset_reset {GPIO_ResetBits(GPIOA,lcd_reset_PIN);}




void LCD12864_GPIO_Config(void);

void u8g2_init(u8g2_t *u8g2);



uint8_t gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);

#endif
