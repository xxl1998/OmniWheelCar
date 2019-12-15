#include "usart3.h"	  
#include "includes.h"

u8  USART3_RX_BUF[USART3_BUF_LEN]; //���ջ���
u8  USART3_TX_BUF[USART3_BUF_LEN]; //���ͻ���
unsigned int USART3_REC_CNT;	

void Usart3_Init(u8 prio)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿�����
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure; 
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	USART_DeInit(USART3);  //��λ����
	  
	#if EN_USART3_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //USART3_TX   PB10   
  #endif 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//USART3_RX   PB11  
  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= prio ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	USART_InitStructure.USART_BaudRate = USART3_BAUD;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	#if EN_USART3_TX
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ   //USART ��ʼ������
  #else 
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;
	#endif
  USART_Init(USART3, &USART_InitStructure); //��ʼ������
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);// USART_IT_RXNE  �����ж�
  USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ��� 

	//RX   ��Ӧ��DMA����
  DMA_DeInit(USART3_DMA_RX_CHANNEL);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;  //DMA����ADC����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_RX_BUF;  //DMA�ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
  DMA_InitStructure.DMA_BufferSize = USART3_BUF_LEN;  //DMAͨ����DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(USART3_DMA_RX_CHANNEL, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
  DMA_Cmd(USART3_DMA_RX_CHANNEL, ENABLE);  //��ʽ����DMA����
	
	#if EN_USART3_TX//TX  DMA  Config
	DMA_DeInit(USART3_DMA_TX_CHANNEL);   //��DMA��ͨ���Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_TX_BUF;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = USART3_BUF_LEN;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(USART3_DMA_TX_CHANNEL, &DMA_InitStructure);  
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA����     
	#endif
}

void USART3_DMA_TX_Enable(unsigned int buf_bytes)//����һ��DMA����
{ 
	DMA_Cmd(USART3_DMA_TX_CHANNEL, DISABLE );  
	USART3_DMA_TX_CHANNEL->CNDTR = buf_bytes;
 	DMA_Cmd(USART3_DMA_TX_CHANNEL, ENABLE); 
}	

void USART3_IRQHandler(void)                	//����1�жϷ������
{
	BaseType_t Result,xHigherPriorityTaskWoken;
	unsigned char reg;
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//idle
  {
		reg=USART3->SR;
		reg=USART3->DR;//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ��     
		reg=reg;
//		USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //����жϱ�־
    Result=xEventGroupSetBitsFromISR(EventGroupHandler, got_USART3_DATA_BIT ,&xHigherPriorityTaskWoken);
		if(Result!=pdFAIL){portYIELD_FROM_ISR(xHigherPriorityTaskWoken);}
		DMA_RX_EN(USART3_DMA_RX_CHANNEL,USART3_BUF_LEN);
	}
}
