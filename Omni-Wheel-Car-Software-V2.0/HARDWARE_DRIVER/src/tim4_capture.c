#include "tim4_capture.h"

void TIM_4_Cap_Init(u8 prepri_TIM_cap,u8 subpri_TIM_cap)
{	
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Period =65535; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =36-1; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision =0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM4, &TIM_ICInitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =prepri_TIM_cap;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =subpri_TIM_cap;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);//����CC1IE�����ж�	TIM_IT_Update|
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��5
}


extern unsigned int encoder_cnt[5];
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{	
		if(PBin(6)==0)encoder_cnt[4]++;
		TIM4->SR = (uint16_t)~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); //����жϱ�־λ
	}
}
