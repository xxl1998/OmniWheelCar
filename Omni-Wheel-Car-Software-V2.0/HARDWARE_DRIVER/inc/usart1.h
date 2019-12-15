#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "includes.h" 

#define USART1_BAUD      115200
#define USART1_BUF_LEN  			30  	//������󻺳��ֽ��� 

extern u8  USART1_RX_BUF[USART1_BUF_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8		USART1_TX_BUF[USART1_BUF_LEN]; 

void Usart1_Init(u8 prio);
void USART1_DMA_TX_Enable(unsigned int buf_bytes);

#define DMA_RX_EN(DMA_CH,LEN);          DMA_Cmd(DMA_CH, DISABLE );\
																					DMA_CH->CNDTR =LEN;\
																					DMA_Cmd(DMA_CH, ENABLE);
																					
#endif
