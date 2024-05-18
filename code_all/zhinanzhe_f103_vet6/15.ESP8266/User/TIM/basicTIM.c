#include  "basicTIM.h"

//��ʱ�� 2-7  (6 7 ������ʱ�� 2345ͨ��)APB1 ���36M
// tim1 �� tim8--�߼���ʱ�� APB2 MAX=72M 
int time=0;

void basic_Tim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	//������ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =TIM_CKD_DIV1; //ʱ�ӷ�Ƶ
	//���ö�ʱ��ʱ�� CK_INT Ƶ���������˲�������ʱ��Ƶ�ʷ�Ƶ�ȣ�������ʱ��û�д˹��ܣ��������á�
	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //������ʽ ������ʱ��ֻ�������ϼ���
	
	TIM_TimeBaseInitStruct.TIM_Period =1000;	//�Զ���װ�ؼĴ�����ֵ
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71; //��ʱ��Ԥ��Ƶ������
	//ʱ��Դ����Ԥ��Ƶ�����Ƕ�ʱ��ʱ�ӣ����趨 TIMx_PSC�Ĵ�����ֵ�������÷�ΧΪ 0 �� 65535��ʵ�� 1 �� 65536 ��Ƶ��
	
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0Xff;
	//�ظ������������ڸ߼����ƼĴ���ר�üĴ���λ�����������Էǳ����׿������ PWM �ĸ���
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	
	//clear ������־λ Clears the TIMx's pending flags.
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	
	
	//Enables or disables the specified TIM interrupts.
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	
	//ʹ�ܼ�����
	TIM_Cmd(TIM6, ENABLE);
	
	 //��ʱ�رն�ʱ����ʱ�ӣ��ȴ�ʹ��
	
}


void basic_Tim_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
/*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); */
	//��0  0bit�����ȼ�(��ռ���ȼ�)   4bit��Ӧ���ȼ�
	//NVIC_PriorityGroup_4         4 bits for pre-emption priority 0 bits for subpriority */
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0; //�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =3; //�����ȼ�
	NVIC_Init(&NVIC_InitStruct);
}

void TIM6_IRQHandler(void) //��ʱ���ж�һ�ε�ʱ���� 1ms
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		time++;
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	
	}

}
