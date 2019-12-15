#include "tim8_pwm.h"
#include "motors.h"

void TIM8_PWM_OUTPUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
                                                             
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7  | GPIO_Pin_8 | GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = TIM8_Period; 
	TIM_TimeBaseStructure.TIM_Prescaler = TIM8_Prescaler; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //add!!!
	TIM_OCInitStructure.TIM_Pulse = TIM8_PWM_MIN; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); 
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); 
	
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); 
	
  TIM_CtrlPWMOutputs(TIM8,ENABLE);	
	TIM_ARRPreloadConfig(TIM8, ENABLE); 
	TIM_Cmd(TIM8, ENABLE); 
}
