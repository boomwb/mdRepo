#include "bsp_adc.h"

__IO uint16_t ADC_Value;

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//ʹ��ADC-GPIOʱ��
	ADC_GPIO_Clock_Fun(ADC_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin  = ADC_PIN;
	GPIO_Init(ADC_PORT,&GPIO_InitStruct);
	
}

static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	//ʹ��ADCʱ��
	ADC_APBxClockFun(ADC_CLK,ENABLE);
	
	// ����ת��ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	
	// ת������Ҷ���
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	
	//// �����ⲿ����ת���������������
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	 //����ģʽ
	ADC_InitStruct.ADC_Mode =ADC_Mode_Independent;
	
	// ת��ͨ�� 1 ��
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADCx,&ADC_InitStruct);
	
	
	//����ADCʱ��CLK2��8��Ƶ ��9M
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //2 4 6 8
	
	// ���� ADC ͨ��ת��˳��Ϊ 1����һ��ת��������ʱ��Ϊ 55.5 ��ʱ������
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL,1,ADC_SampleTime_55Cycles5);
	
	//ADCת������������ж�
	ADC_ITConfig(ADCx,ADC_IT_EOC,ENABLE);
	
	//����ADC,����ʼת��
	ADC_Cmd(ADCx,ENABLE);
	
	//resets the selected ADC calibration registers.
	ADC_ResetCalibration(ADCx);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx));	
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx);
	// �ȴ�У׼���
	while( ADC_GetCalibrationStatus(ADCx) );
	
	// ����û�в����ⲿ����������ʹ��������� ADC ת��
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

