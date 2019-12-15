#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x_usart.h" 
#include "stm32f10x_dma.h"  	

#define USART2_BUF_LEN 500      //TX & RX
extern u8  USART2_RX_BUF[USART2_BUF_LEN]; //Ω” ’ª∫≥Â
extern u8  USART2_TX_BUF[USART2_BUF_LEN]; //∑¢ÀÕª∫≥Â
extern unsigned int USART2_REC_CNT;	
#define USART2_DMA_RX_CHANNEL   DMA1_Channel6
#define USART2_DMA_TX_CHANNEL   DMA1_Channel7
#define EN_USART2_TX   1
#define EN_USART2_RX   1
#define USART2_BAUD    115200

void Usart2_Init(u8 prio);
void USART2_DMA_TX_Enable(unsigned int buf_bytes);

#endif
