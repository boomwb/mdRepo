#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "oled.h"
#include "sys.h"

#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "key.h"


#include "spi.h"	  


u8 state=0;

void xianshi(void);//显示信息  
void refshow(void);//刷新显示
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色 
}
void xianshi()//显示信息
{ 
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;   
	//显示32*32汉字
	showhanzi32(0,0,0);	 //淘
	showhanzi32(40,0,1);	 //晶
	showhanzi32(80,0,2);    //驰
	//显示16*16汉字
	showhanzi16(0,35,0);	  //专
	showhanzi16(20,35,1);	  //注
	showhanzi16(40,35,2);	  //显
	showhanzi16(60,35,3);	  //示
	showhanzi16(80,35,4);	  //方
	showhanzi16(100,35,5);	  //案	   
	LCD_ShowString(0,55,200,16,16,"1.8 TFT SPI");
}
void showqq()
{ 
	u16 x,y; 
	x=0;
	y=75;
	while(y<lcddev.height-39)
	{
		x=0;
		while(x<lcddev.width-39)
		{
			showimage(x,y);	
			x+=40;
		}
		y+=40;
	 }	  
}
void refshow(void)	 //刷新显示
{
	switch(state)
	{
		case 0:
		LCD_Clear(WHITE);
	    xianshi();
    	showqq();
		break;
		case 1:
		LCD_Clear(BLACK);	
		break;
		case 2:
		LCD_Clear(RED);
		break;
		case 3:
		LCD_Clear(GREEN);
		break;
		case 4:
		LCD_Clear(BLUE);
		break;
	}	
}
void jiance(u8 key) //检测按键
{ 	
	if(key==2)
	{
		state++;
		LCD_Init();
		if(state==5)
		{
			state=0; 
		}
		refshow();//刷新显示
	}

} 

int main(void)
{	

	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	USART_Config();
	printf(" \r\n 8M byte字节 串行flash实验 \r\n");
	/* 中断方式需要sysTick_Init */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms
	/*  计数模式 不需要 */
	//	SysTick_Delay_Ms(200);
	   	 
	 	Key_GPIO_Config();
	SPI1_Init();	//SPI1初始化
	 
	 
	LCD_Init();	  			
 //	POINT_COLOR=RED;//设置字体为红色 
	//xianshi();	   //显示信息
	//showqq();	   //显示QQ	
	POINT_COLOR=BLUE;
		//LCD_Color_Fill(10,10,20,20,(uint16_t*)BLUE);
		
		Draw_Circle(30,40,20);
	while(1)
	{
//		Load_Drow_Dialog();
//		SysTick_Delay_Ms(1000);
		LCD_Fill(0,0,127,159,BLUE);
		SysTick_Delay_Ms(2000);
		LCD_Fill(0,0,127,159,RED);
		SysTick_Delay_Ms(2000);
		LCD_Fill(0,0,127,159,0x4868);
		SysTick_Delay_Ms(1500);
	}
}


