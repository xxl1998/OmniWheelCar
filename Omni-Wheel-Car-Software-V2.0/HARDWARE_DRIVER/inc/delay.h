#ifndef __DELAY_H
#define __DELAY_H 			   
#include "includes.h"  

	 
void delay_init(void);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);

u32 getSysTickCnt(void);	

#endif





























