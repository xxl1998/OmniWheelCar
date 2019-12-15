#ifndef __SPI1_H
#define __SPI1_H
#include "includes.h"


 
 				  	    													  
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节




u8 SPI1_Read_Byte(u8 reg,GPIO_TypeDef * CS_PORT,uint16_t CS_IO);
u8 SPI1_Write_Byte(u8 reg,u8 data,GPIO_TypeDef * CS_PORT,uint16_t CS_IO);
u8 SPI1_Read_Len(u8 reg,u8 len,u8 *buf,GPIO_TypeDef * CS_PORT,uint16_t CS_IO);
void CS_IO_Init(GPIO_TypeDef* GPIOx,uint16_t pin,uint32_t CS_APB2Periph_CLK);



#endif

