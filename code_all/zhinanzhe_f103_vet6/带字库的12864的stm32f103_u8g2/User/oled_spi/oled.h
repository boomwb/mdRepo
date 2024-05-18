#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"



		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS 	PCout(4)
#define OLED_RST  	PAout(7)	
#define OLED_RS 	PEout(6) //DC

 
//使用4线串行接口时使用 
#define OLED_SCLK 	PAout(4)
#define OLED_SDIN 	PAout(5)
		     
#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据



#endif  
	 



