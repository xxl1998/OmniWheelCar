#include "usart2.h"	 
#include "includes.h"

u8  USART2_RX_BUF[USART2_BUF_LEN]; //接收缓冲
u8  USART2_TX_BUF[USART2_BUF_LEN]; //发送缓冲
unsigned int USART2_REC_CNT;	

void USART2_IRQHandler(void)                	//串口中断服务程序
{
	BaseType_t Result,xHigherPriorityTaskWoken;
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
 {
		USART_ReceiveData(USART2);//读取数据 注意：这句必须要，否则不能够清除中断标志位。
    USART2_REC_CNT  = USART2_BUF_LEN- USART2_DMA_RX_CHANNEL->CNDTR ;
				
				DMA_Cmd(USART2_DMA_RX_CHANNEL, DISABLE );
				USART2_DMA_RX_CHANNEL->CNDTR = USART2_BUF_LEN;
				DMA_Cmd(USART2_DMA_RX_CHANNEL, ENABLE);//恢复DMA指针，等待下一次的接收

       Result=xEventGroupSetBitsFromISR(EventGroupHandler, got_USART2_DATA_BIT ,&xHigherPriorityTaskWoken);
			if(Result!=pdFAIL){portYIELD_FROM_ISR(xHigherPriorityTaskWoken);}

	} 
} 

void Usart2_Init(u8 prio)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口设置
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure; 
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	USART_DeInit(USART2);  //复位串口
	  
	#if EN_USART2_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //USART2_TX   PA2 
  #endif 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//USART2_RX   PA3
  
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= prio ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	USART_InitStructure.USART_BaudRate = USART2_BAUD;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	#if EN_USART2_TX
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式   //USART 初始化设置
  #else 
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;
	#endif
  USART_Init(USART2, &USART_InitStructure); //初始化串口
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);// USART_IT_RXNE  开启中断
  USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART2, ENABLE);  //使能串口 

	//RX   相应的DMA配置
  DMA_DeInit(USART2_DMA_RX_CHANNEL);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设ADC基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART2_RX_BUF;  //DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
  DMA_InitStructure.DMA_BufferSize = USART2_BUF_LEN;  //DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
  DMA_Init(USART2_DMA_RX_CHANNEL, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
  DMA_Cmd(USART2_DMA_RX_CHANNEL, ENABLE);  //正式驱动DMA传输
	
	#if EN_USART2_TX//TX  DMA  Config
	DMA_DeInit(USART2_DMA_TX_CHANNEL);   //将DMA的通道寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART2_TX_BUF;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = USART2_BUF_LEN;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(USART2_DMA_TX_CHANNEL, &DMA_InitStructure);  
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送     
	#endif  
}

void USART2_DMA_TX_Enable(unsigned int buf_bytes)//开启一次DMA传输
{ 
	DMA_Cmd(USART2_DMA_TX_CHANNEL, DISABLE );  
	USART2_DMA_TX_CHANNEL->CNDTR = buf_bytes;
 	DMA_Cmd(USART2_DMA_TX_CHANNEL, ENABLE); 
}	
