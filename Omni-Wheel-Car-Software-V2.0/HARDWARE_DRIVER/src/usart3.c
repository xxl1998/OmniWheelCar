#include "usart3.h"	  
#include "includes.h"

u8  USART3_RX_BUF[USART3_BUF_LEN]; //接收缓冲
u8  USART3_TX_BUF[USART3_BUF_LEN]; //发送缓冲
unsigned int USART3_REC_CNT;	

void Usart3_Init(u8 prio)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口设置
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure; 
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	USART_DeInit(USART3);  //复位串口
	  
	#if EN_USART3_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //USART3_TX   PB10   
  #endif 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);//USART3_RX   PB11  
  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= prio ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	USART_InitStructure.USART_BaudRate = USART3_BAUD;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	#if EN_USART3_TX
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式   //USART 初始化设置
  #else 
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;
	#endif
  USART_Init(USART3, &USART_InitStructure); //初始化串口
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);// USART_IT_RXNE  开启中断
  USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART3, ENABLE);  //使能串口 

	//RX   相应的DMA配置
  DMA_DeInit(USART3_DMA_RX_CHANNEL);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;  //DMA外设ADC基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_RX_BUF;  //DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
  DMA_InitStructure.DMA_BufferSize = USART3_BUF_LEN;  //DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
  DMA_Init(USART3_DMA_RX_CHANNEL, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
  DMA_Cmd(USART3_DMA_RX_CHANNEL, ENABLE);  //正式驱动DMA传输
	
	#if EN_USART3_TX//TX  DMA  Config
	DMA_DeInit(USART3_DMA_TX_CHANNEL);   //将DMA的通道寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_TX_BUF;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = USART3_BUF_LEN;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(USART3_DMA_TX_CHANNEL, &DMA_InitStructure);  
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送     
	#endif
}

void USART3_DMA_TX_Enable(unsigned int buf_bytes)//开启一次DMA传输
{ 
	DMA_Cmd(USART3_DMA_TX_CHANNEL, DISABLE );  
	USART3_DMA_TX_CHANNEL->CNDTR = buf_bytes;
 	DMA_Cmd(USART3_DMA_TX_CHANNEL, ENABLE); 
}	

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	BaseType_t Result,xHigherPriorityTaskWoken;
	unsigned char reg;
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)//idle
  {
		reg=USART3->SR;
		reg=USART3->DR;//读取数据 注意：这句必须要，否则不能够清除中断标志位。     
		reg=reg;
//		USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //清除中断标志
    Result=xEventGroupSetBitsFromISR(EventGroupHandler, got_USART3_DATA_BIT ,&xHigherPriorityTaskWoken);
		if(Result!=pdFAIL){portYIELD_FROM_ISR(xHigherPriorityTaskWoken);}
		DMA_RX_EN(USART3_DMA_RX_CHANNEL,USART3_BUF_LEN);
	}
}
