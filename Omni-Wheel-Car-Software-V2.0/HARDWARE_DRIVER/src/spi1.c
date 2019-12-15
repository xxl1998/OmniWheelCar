#include "spi1.h"

void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );//PORTAʱ��ʹ�� 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);  //����
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������		 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ  
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 
} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

//������ reg:Ҫ��ȡ�ļĴ�����ַ  len:Ҫ��ȡ�ĳ���   buf:��ȡ�������ݴ洢��
u8 SPI1_Read_Len(u8 reg,u8 len,u8 *buf,GPIO_TypeDef * CS_PORT,uint16_t CS_IO)
{  
	u8 i=0;
 	CS_PORT->BRR = CS_IO;                          //ʹ������   	
	SPI1_ReadWriteByte(reg|0x80);    //���Ͷ�ȡ״̬�Ĵ�������    
	for(i=0;i<len-1;i++)buf[i]=SPI1_ReadWriteByte(reg|0x80+i+1);  //��ȡһ���ֽ�  
	buf[len-1]=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	CS_PORT->BSRR = CS_IO ;                             //ȡ��Ƭѡ    
	return 0;		
}
//дһ���ֽ� 
u8 SPI1_Write_Byte(u8 reg,u8 data,GPIO_TypeDef * CS_PORT,uint16_t CS_IO) 				 
{
	CS_PORT->BRR = CS_IO;                     //ʹ������   
	SPI1_ReadWriteByte(reg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(data);               //д��һ���ֽ�  
	CS_PORT->BSRR = CS_IO ;       
	return 0;
}
//��һ���ֽ�   reg:�Ĵ�����ַ 
u8 SPI1_Read_Byte(u8 reg,GPIO_TypeDef * CS_PORT,uint16_t CS_IO)
{
	u8 byte=0;
	CS_PORT->BRR = CS_IO;                       //ʹ������   
	SPI1_ReadWriteByte(reg|0x80);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	CS_PORT->BSRR = CS_IO ;                            //ȡ��Ƭѡ     
	return byte;   
}

void CS_IO_Init(GPIO_TypeDef* GPIOx,uint16_t pin,uint32_t CS_APB2Periph_CLK)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(CS_APB2Periph_CLK,ENABLE);	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStruct);
	
	GPIOx->BSRR = pin ;  
	delay_ms(1);
}




uint8_t spi2_write_reg(uint8_t reg_addr,uint8_t reg_val)
{
	SPI1_ReadWriteByte(reg_addr&0x7f);
	SPI1_ReadWriteByte(reg_val);
	return 0;
}


uint8_t spi2_read_reg(uint8_t reg_addr)
{
	SPI1_ReadWriteByte(reg_addr|0x80);
	return SPI1_ReadWriteByte(0xff);
}
uint8_t spi2_read_reg_buffer(uint8_t reg_addr,void *buffer,uint16_t len)
{
	uint8_t *p = buffer;
	uint16_t i;
	SPI1_ReadWriteByte(reg_addr|0x80);
	for(i=0;i<len;i++)
	{
		*p++= SPI1_ReadWriteByte(0xff);
	}
	return 0;
}
