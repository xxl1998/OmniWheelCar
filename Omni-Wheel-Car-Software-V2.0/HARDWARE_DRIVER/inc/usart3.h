#ifndef __USART3_H
#define __USART3_H	

#include "stm32f10x_usart.h" 
#include "stm32f10x_dma.h"  

#define USART3_BUF_LEN 9      //TX & RX
extern u8  USART3_RX_BUF[USART3_BUF_LEN]; //接收缓冲
extern u8  USART3_TX_BUF[USART3_BUF_LEN]; //发送缓冲
extern unsigned int USART3_REC_CNT;	
#define USART3_DMA_RX_CHANNEL   DMA1_Channel3
#define USART3_DMA_TX_CHANNEL   DMA1_Channel2
#define EN_USART3_TX   1
#define EN_USART3_RX   1
#define USART3_BAUD    19200

void Usart3_Init(u8 prio);
void USART3_DMA_TX_Enable(unsigned int buf_bytes);

#define DMA_RX_EN(DMA_CH,LEN);          DMA_Cmd(DMA_CH, DISABLE );\
																					DMA_CH->CNDTR =LEN;\
																					DMA_Cmd(DMA_CH, ENABLE);
																					//恢复DMA指针，等待下一次的接收
#endif      
