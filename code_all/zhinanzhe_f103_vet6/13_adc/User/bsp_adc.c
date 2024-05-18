#include "bsp_adc.h"

__IO uint16_t ADC_Value;

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//使能ADC-GPIO时钟
	ADC_GPIO_Clock_Fun(ADC_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin  = ADC_PIN;
	GPIO_Init(ADC_PORT,&GPIO_InitStruct);
	
}

static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	//使能ADC时钟
	ADC_APBxClockFun(ADC_CLK,ENABLE);
	
	// 连续转换模式
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	
	// 转换结果右对齐
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	
	//// 不用外部触发转换，软件开启即可
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	 //独立模式
	ADC_InitStruct.ADC_Mode =ADC_Mode_Independent;
	
	// 转换通道 1 个
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADCx,&ADC_InitStruct);
	
	
	//配置ADC时钟CLK2的8分频 即9M
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //2 4 6 8
	
	// 配置 ADC 通道转换顺序为 1，第一个转换，采样时间为 55.5 个时钟周期
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL,1,ADC_SampleTime_55Cycles5);
	
	//ADC转换结束后产生中断
	ADC_ITConfig(ADCx,ADC_IT_EOC,ENABLE);
	
	//开启ADC,并开始转换
	ADC_Cmd(ADCx,ENABLE);
	
	//resets the selected ADC calibration registers.
	ADC_ResetCalibration(ADCx);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADCx));	
	
	// ADC开始校准
	ADC_StartCalibration(ADCx);
	// 等待校准完成
	while( ADC_GetCalibrationStatus(ADCx) );
	
	// 由于没有采用外部触发，所以使用软件触发 ADC 转换
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
}


static void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =1;
	
	NVIC_Init(&NVIC_InitStruct);
}
 
void ADCx_Init(void)
{
	ADCx_GPIO_Config();

	ADCx_Mode_Config();
	
	ADC_NVIC_Config();
}


//void ADC_IRQHandler(void)
//{
//	if(ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET)
//	{
//		ADC_Value = ADC_GetConversionValue(ADCx);
//	}
//	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
//}

