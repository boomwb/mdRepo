#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "sys.h"
#include  "basicTIM.h"
#define LA PAout(4)
#define LB PAout(5)
#define LC PAout(6)
#define LD PAout(7)

extern int time;
uint8_t phaseccw[4]={0x08,0x04,0x02,0x01};

uint8_t steps[8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09};
typedef enum _PIN_BIT_
{
	PLA = 0x01,
	PLB =0x02,
	PLC = 0x04,
	PLD = 0x08,
}Pin_Bit;

typedef struct
{
	float target_val;
	float actual_val;
	float err; //定义偏差
	float err_last; //定义上一个偏差
	float Kp,Ki,Kd; //p比例 i积分 d微分
	float integral ;// 积分值
}_pid;

_pid pid;

//PID 参数初始化
void PID_param_init()
{
	pid.target_val = 0.0;
	pid.actual_val = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.integral = 0.0;
	pid.Kp = 0.31;
	pid.Ki = 0.070;
	pid.Kd = 0.3;
}









//PID 算法实现  retval: 通过 PID 计算后的输出
float PID_realize(float temp_val)
{
	/* 计算目标值与实际值的误差 */
	pid.err=pid.target_val-temp_val;

	/* 误差累积 */
	pid.integral+=pid.err;

	/*PID 算法实现 */
	pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);

	/* 误差传递 */
	pid.err_last=pid.err;

	/* 返回当前实际值 */
	return pid.actual_val;	

}

/**
定时器周期调用函数
这个函数主要在定时器中断中调用，定时器配置为每 20ms 中断一次，PID 算法每 20ms 执行一
次，这也就是算法的周期。
*/
int pid_status;
void time_period_fun()
{

	static int num=0;

	static int run_i=0;

	if(!pid_status)
	{
		float val=PID_realize(pid.actual_val);
		int temp = val;
		// 给通道 1 发送实际值
		//set_computer_value(SEED_FACT_CMD, CURVES_CH1, &temp, 1);
	}

}








//正转
void Motor_CW(void)
{
#if 0
	PAout(4)=1; PAout(5)=0; PAout(6)=0; PAout(7)=0; SysTick_Delay_Ms(15);
	PAout(4)=0; PAout(5)=1; PAout(6)=0; PAout(7)=0; SysTick_Delay_Ms(15);
	PAout(4)=0; PAout(5)=0; PAout(6)=1; PAout(7)=0; SysTick_Delay_Ms(15);
	PAout(4)=0; PAout(5)=0; PAout(6)=0; PAout(7)=1; SysTick_Delay_Ms(15);
#else
	uint8_t temp;
	for(int i=0;i<8;i++)
	{
		temp = steps[i];
		LA= (temp&PLA)>>0;
		LB= (temp&PLB)>>1;
		LC= (temp&PLC)>>2;
		LD= (temp&PLD)>>3;
		SysTick_Delay_Us(990);
		//SysTick_Delay_Ms(1);
	}

#endif
	
}





int main(void)
{	

	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	
	USART_Config();
	Motor_Init();
	/* 中断方式 */
	//	SysTick_Init();
	//	Delay_us(100); //100*10us=1ms

	//SysTick_Delay_Ms(15);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

	basic_Tim_Config();
	
	basic_Tim_NVIC_Config();
	
	

	
	while(1)
	{
		Motor_CW();
	
//		if(time==1000)
//		{
//			//PBout(0)^=1;
//			PBout(0)=~PBout(0);
//			//LED3_TOGGLE;
//			time=0;
//		}
	
	
	}
		
	
}


