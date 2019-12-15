#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "FreeRTOS.h"					//FreeRTOS使用		   
#include "task.h" 
#include "event_groups.h" 

#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "misc.h" 
#include <math.h>

#include "delay.h"
#include "sys.h"
#include "my_config.h"

#include "usart1.h"
#include "tim2.h"



extern void*  EventGroupHandler;	//事件标志组句柄

#endif 

