#include "motors.h"
#include "tim8_pwm.h"
#include "stm32f10x_gpio.h"
//  220 pulses per circule
void Set_Motor_PWM(unsigned char isMotorx,unsigned char direction,unsigned short Speedx)
{
	if(Speedx>TIM8_PWM_MAX)Speedx=TIM8_PWM_MAX;
	if(Speedx<=TIM8_PWM_MIN)Speedx=TIM8_PWM_MIN;
	
	switch (isMotorx)
	{
		case M1:
			if(Speedx!=0)
			{
				if(direction==0)
				{
					M1_Ctrl_IO_1=0;
					M1_Ctrl_IO_2=1;
				}
				else if(direction==1)
				{
					M1_Ctrl_IO_1=1;
					M1_Ctrl_IO_2=0;
				}
			}
			else 
			{
				M1_Ctrl_IO_1=0;
				M1_Ctrl_IO_2=0;
			}
			M1_PWM = Speedx;
			break;

		case M2:
			if(Speedx!=0)
			{
				if(direction==0)
				{
					M2_Ctrl_IO_1=0;
					M2_Ctrl_IO_2=1;
				}
				else if(direction==1)
				{
					M2_Ctrl_IO_1=1;
					M2_Ctrl_IO_2=0;
				}
			}
			else 
			{
				M2_Ctrl_IO_1=0;
				M2_Ctrl_IO_2=0;
			}
			M2_PWM = Speedx;
			break;
			
		case M3:
			if(Speedx!=0)
			{
				if(direction==0)
				{
					M3_Ctrl_IO_1=0;
					M3_Ctrl_IO_2=1;
				}
				else if(direction==1)
				{
					M3_Ctrl_IO_1=1;
					M3_Ctrl_IO_2=0;
				}
			}
			else 
			{
				M3_Ctrl_IO_1=0;
				M3_Ctrl_IO_2=0;
			}
			M3_PWM = Speedx;
			break;
			
		case M4:
			if(Speedx!=0)
			{
				if(direction==0)
				{
					M4_Ctrl_IO_1=0;
					M4_Ctrl_IO_2=1;
				}
				else if(direction==1)
				{
					M4_Ctrl_IO_1=1;
					M4_Ctrl_IO_2=0;
				}
			}
			else 
			{
				M4_Ctrl_IO_1=0;
				M4_Ctrl_IO_2=0;
			}
			M4_PWM = Speedx;
			break;
			
		default:break;
	}
}

void Motors_Init(void)
{
	TIM8_PWM_OUTPUT_Init();
	Motors_Ctrl_IO_Init();
	
}

/*********main.c  IO map  ****************/
void Motors_Ctrl_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|   \
	                     RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	 
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	 GPIO_ResetBits(GPIOA,GPIO_Pin_4);				

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_12;	
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
	 GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_12);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
	 GPIO_ResetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3 );
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	 GPIO_Init(GPIOD, &GPIO_InitStructure);					 
	 GPIO_ResetBits(GPIOD, GPIO_Pin_2 );
}
