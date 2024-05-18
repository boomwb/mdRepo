#include "bsp_u8g2.h"
//#include "font.h"

void LCD12864_GPIO_Config(void)
{		
	
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);
	
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;	 //4
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*设置引脚模式为通用推挽输出*/    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_7; //3
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_8 | GPIO_Pin_9; //5
		GPIO_Init(GPIOC, &GPIO_InitStructure);

	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
		GPIO_Init(GPIOE, &GPIO_InitStructure);	
}



void u8g2_init(u8g2_t *u8g2)
{
	u8g2_Setup_st7565_jlx12864_f(u8g2, U8G2_R2, u8x8_byte_8bit_6800mode, gpio_and_delay_cb);	 //U8G2_R0反转

	u8g2_InitDisplay(u8g2);        // 显示器处于关闭状态
    u8g2_SetPowerSave(u8g2, 0);    // wake up display
	u8g2_ClearBuffer(u8g2);

	
	u8g2_SetContrast(u8g2, 150);  //设置对比度	

	
	u8g2_FirstPage(u8g2);
	do
	{
		u8g2_SetFont(u8g2, u8g2_font_ASCII_fonts);
		u8g2_DrawStr(u8g2, 30,30,"U8g2");
	
	}while(u8g2_NextPage(u8g2));

}


uint8_t gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch(msg)
  {
    //Initialize SPI peripheral
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		//gpio_Init();
		break;
	//Function which implements a delay, arg_int contains the amount of ms
	case U8X8_MSG_DELAY_MILLI:
		SysTick_Delay_Ms(arg_int);
		break;
	//Function which delays 10us
    case U8X8_MSG_DELAY_10MICRO:
		SysTick_Delay_Us(arg_int);
		break;
    //Function to define the logic level of the clockline
	case U8X8_MSG_GPIO_CS:
        if(arg_int)  {lcd_cs1_set;}
		else {lcd_cs1_reset;}
		break;
    //Function to define the logic level of the data line to the display
	case U8X8_MSG_GPIO_DC:
		if(arg_int) 
		{
			lcd_rs_set;		
		} 
		else 
		{
			lcd_rs_reset;
		}
		break;
    // Function to define the logic level of the CS line
	case U8X8_MSG_GPIO_E:
		if(arg_int) {lcd_e_set;} 
		else  {lcd_e_reset;}
		break;
	//Function to define the logic level of the Data/ Command line
	case U8X8_MSG_GPIO_D7:
		if(arg_int)  {GPIO_SetBits(GPIOB,GPIO_Pin_6);}
		else   { GPIO_ResetBits(GPIOB,GPIO_Pin_6);}
		break;
	case U8X8_MSG_GPIO_D6:
		if(arg_int)  {GPIO_SetBits(GPIOC,GPIO_Pin_13);}
		else   { GPIO_ResetBits(GPIOC,GPIO_Pin_13);}
		break;
	 case U8X8_MSG_GPIO_D5:
		if(arg_int) {GPIO_SetBits(GPIOE,GPIO_Pin_5);}
		else {GPIO_ResetBits(GPIOE,GPIO_Pin_5);}
		break;
	 case U8X8_MSG_GPIO_D4:
		if(arg_int) {GPIO_SetBits(GPIOA,GPIO_Pin_6);}
		else {GPIO_ResetBits(GPIOA,GPIO_Pin_6);}
		break;		 
	 case U8X8_MSG_GPIO_D3:
		if(arg_int) {GPIO_SetBits(GPIOA,GPIO_Pin_4);}
		else {GPIO_ResetBits(GPIOA,GPIO_Pin_4);}
		break;			 
	 case U8X8_MSG_GPIO_D2:
		if(arg_int) {GPIO_SetBits(GPIOB,GPIO_Pin_8);}
		else {GPIO_ResetBits(GPIOB,GPIO_Pin_8);}
		break;		 
	 case U8X8_MSG_GPIO_D1:
		if(arg_int) {GPIO_SetBits(GPIOC,GPIO_Pin_11);}
		else {GPIO_ResetBits(GPIOC,GPIO_Pin_11);}
		break;
	 case U8X8_MSG_GPIO_D0:
		if(arg_int) {GPIO_SetBits(GPIOC,GPIO_Pin_10);}
		else {GPIO_ResetBits(GPIOC,GPIO_Pin_10);}
		break;
  	 case U8X8_MSG_GPIO_MENU_HOME:
		u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
		break;
	//Function to define the logic level of the RESET line
	 case U8X8_MSG_GPIO_RESET:
	   if(arg_int) {lcd_reset_set;}
	   else  {lcd_reset_reset}
	   break;
	
	default:
	  return 0; //A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}



///* --spi接口设置-- u8g2 */
//uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,U8X8_UNUSED void *arg_ptr)
//{
//	switch(msg)
//	{
//		case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
//				__NOP();
//				break;
//		case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
//				delay1ms(10);
//				break;
//		case U8X8_MSG_DELAY_MILLI:			                // delay arg_int * 1 milli second
//				delay1ms(arg_int);
//				break;
//									   
//		case U8X8_MSG_GPIO_SPI_DATA:
//			  if(arg_int == 1) 
//			  {
//				  SDA_High();
//			  }				  // arg_int=1: Input dir with pullup high for I2C clock pin
//			  else if(arg_int == 0)
//			  {
//				  SDA_Low();
//			  }
//			break;
//		case U8X8_MSG_GPIO_SPI_CLOCK:
//			if(arg_int == 1)                                     // arg_int=1: Input dir with pullup high for I2C clock pin
//			{
//				SCLK_High();
//			}
//			else if(arg_int == 0)
//			{
//				SCLK_Low();
//			}
//			break;
//		case U8X8_MSG_GPIO_CS:
//			if(arg_int == 1)   
//			{
//				CS1_High();
//			}				  // arg_int=1: Input dir with pullup high for I2C clock pin
//			else if(arg_int == 0)
//			{
//				CS1_Low();
//			}
//			break;
//		case U8X8_MSG_GPIO_DC:
//			if(arg_int == 1)                                     // arg_int=1: Input dir with pullup high for I2C clock pin
//			{
//				RS_High();
//			}
//			else if(arg_int == 0)
//			{
//				RS_Low();
//			}
//			break;
//		case U8X8_MSG_GPIO_RESET:
//			if(arg_int == 1)                               
//			{
//				RESET_High();
//			}
//			else if(arg_int == 0)
//			{
//				RESET_Low();
//			}
//			break;
//		case U8X8_MSG_GPIO_MENU_SELECT:
//			u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
//			break;
//		case U8X8_MSG_GPIO_MENU_NEXT:
//			u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
//			break;
//		case U8X8_MSG_GPIO_MENU_PREV:
//			u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
//			break;
//		case U8X8_MSG_GPIO_MENU_HOME:
//			u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
//			break;
//		default:
//			u8x8_SetGPIOResult(u8x8, 1);			        
//			break;
//		}
//	return 1;
//}



/* u8g2 */


