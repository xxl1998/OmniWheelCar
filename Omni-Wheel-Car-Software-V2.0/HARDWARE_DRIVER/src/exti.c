#include "exti.h"
#include "stm32f10x_exti.h"
#include "delay.h"
/*******TIM5 CH1  PA0  
				TIM4 CH1  PB6
				TIM2 CH2  PA1
				TIM3 CH1  PA6***********/
//外部中断0服务程序 
unsigned int encoder_cnt[5];
void EXTI0_IRQHandler(void)
{
	if(PAin(0)==0)encoder_cnt[2]++;
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}

void EXTI1_IRQHandler(void)
{
	if(PAin(1)==0)encoder_cnt[1]++;
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE1上的中断标志位  
}

void EXTI9_5_IRQHandler(void)
{
	//if(PBin(6)==0)encoder_cnt[2]++;
	if(PAin(6)==0)encoder_cnt[3]++;
	EXTI_ClearITPendingBit(EXTI_Line6);  //清除LINE6上的中断标志位  
}

void EXTIX_Init(void)
{ 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    Encoder_IO_Init();	 //	 

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


 
/*******TIM5 CH1  PA0  
				TIM4 CH1  PB6
				TIM2 CH2  PA1
				TIM3 CH1  PA6***********/
void Encoder_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//
}
