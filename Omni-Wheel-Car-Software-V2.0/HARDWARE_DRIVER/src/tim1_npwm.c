#include "tim1_npwm.h"
/****TIM1
CH1N PB13
CH2N PB14
CH3N PB15  */
#define  TIM1_NPWM_MIN   200
#define PWM_Period  1000-1
#define   PWM_Prescaler   72-1
void TIM1_NPWM_OUTPUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOB, ENABLE); 

	/*********IO初始化*******/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	/*********TIM1初始化*******/
  TIM_TimeBaseStructure.TIM_Period = PWM_Period ;       //一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = PWM_Prescaler ;	    //设置预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	
	/*********TIM1 CH1 CH2 CH3 CH4初始化*******/	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;    
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = TIM1_NPWM_MIN;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平            
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;   
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; 
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);	 //使能通道
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//Motor2

  TIM_OC2Init(TIM1, &TIM_OCInitStructure);	//使能通道
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);//Motor3
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);	//使能通道
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);//Motor3
	
  TIM_ARRPreloadConfig(TIM1, ENABLE);			 // 使能TIM重载寄存器ARR
  TIM_CtrlPWMOutputs(TIM1, ENABLE);   //TIM1 高级定时器，多一句
	TIM_Cmd(TIM1, ENABLE);                   //使能定时器
}
