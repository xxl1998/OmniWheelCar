#ifndef __LEDTASK_H
#define __LEDTASK_H	 
#include "includes.h"

#define  RGBLED_R  TIM1->CCR1
#define  RGBLED_G  TIM1->CCR2
#define  RGBLED_B  TIM1->CCR3

void LedTask(void *pvParameters);
		 				    
#endif
