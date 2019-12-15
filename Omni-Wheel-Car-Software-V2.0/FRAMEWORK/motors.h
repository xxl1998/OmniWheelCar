#ifndef __MOTORS_
#define __MOTORS_
#include "sys.h"
/*********Ïê¼ûmain.c   ****************/
#define  M1_Ctrl_IO_1      PCout(3)
#define  M1_Ctrl_IO_2      PCout(2)
#define  M2_Ctrl_IO_1      PBout(4)
#define  M2_Ctrl_IO_2      PBout(8)
#define  M3_Ctrl_IO_1      PBout(12)
#define  M3_Ctrl_IO_2      PAout(4)
#define  M4_Ctrl_IO_1      PBout(3)
#define  M4_Ctrl_IO_2      PDout(2)


#define  M1_PWM       TIM8->CCR2
#define  M2_PWM       TIM8->CCR3
#define  M3_PWM       TIM8->CCR1
#define  M4_PWM       TIM8->CCR4


#define   Anticlockwise   1
#define   Clockwise       0


#define   TIM8_Prescaler    72-1
#define   TIM8_Period       1000-1
#define   TIM8_PWM_MIN     0
#define   TIM8_PWM_MAX    TIM8_Period

#define   M1    (1<<1)
#define   M2    (1<<2)
#define   M3    (1<<3)
#define   M4    (1<<4)

void Motors_Init(void);
void Motors_Ctrl_IO_Init(void);
void Set_Motor_PWM(unsigned char isMotorx,unsigned char direction,unsigned short Speedx);


#endif
