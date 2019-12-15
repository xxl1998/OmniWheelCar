#ifndef __EXTI_H
#define __EXTI_H	 
#include "sys.h"

extern unsigned int encoder_cnt[5];

void Encoder_IO_Init(void);
void EXTIX_Init(void);//外部中断初始化		 					    
#endif

