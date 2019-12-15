#include "adc.h"
#include "delay.h"		 
#include "stdio.h"

__IO uint16_t ADC_RawValue[10]; //���ڴ洢DMA��������Ļ�������(12ͨ����ÿͨ����10��ֵ)
__IO float  ConvertedVoltage; //���ƽ���˲����ADCֵ
__IO unsigned int ADC_FilteredRaw;

void ADC_Get_Voltage(void)
{
	uint8_t i=0;
	ADC_FilteredRaw=0;
	//��10��ֵ�����м򵥵�ƽ���˲�
	for(i=0; i<10; i++)
	{
		ADC_FilteredRaw += (ADC_RawValue[i] & 0x0fff); //ȷ��ȡ�õ�12λ��Ч��ADC����
  }
	ADC_FilteredRaw /=10;
		
	//���Դ�ӡADCֵ��Ӧ�ĵ�ѹֵ
	ConvertedVoltage= ADC_FilteredRaw*3.32f/4096.0f;
	//printf("ADC = raw:%d --- %fV\r\n", ADC_FilteredRaw, ConvertedVoltage);
}

//��ʼ��ADC															   
void ADC10_Init(void)
{    
  ADC_InitTypeDef ADC_InitStructure;       
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO�˿�����
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/* 1. ADCʱ������ */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //��ADCʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //����ʱ��(12MHz),F1������14MHZ
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//��DMAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//��GPIOʱ��
	
	/* 2. ADC��GPIO��ʼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//GPIOC
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
	/* 3. ADC�������� */
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //��������ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //���ⲿ����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1��ADC2����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1; //ͨ����
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //��������ɨ��ģʽ 
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ��� 
	
	/* 4. ADCͨ������ */
	//�������õ�ADCCLK=12MHz,55.5���ڵĲ���ʱ��
	//��TCONV=55.5+12.5=68������=68/12MHz=5.67us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10,11, ADC_SampleTime_239Cycles5);
    
	/* 5. DMA��ʼ�� */
	DMA_DeInit(DMA1_Channel1); //�ȸ�λ��ӦDMAͨ���ļĴ���ֵ
	
	//��Ӧ��DMA����
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //����Դ��ַ  ADC���ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_RawValue; //�ڴ��д洢DMA������������ݻ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = 10; //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //��ʹ���ڴ浽�ڴ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //����Ĵ�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַҪ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA�������ݿ�� 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�ڴ�����Ϊ���� 16λ��ADC����Ϊ12λ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMA������ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA���ȼ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //DMA��ʼ��
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMA
	
    /* 6. ʹ��ADC */
    ADC_Cmd(ADC1, ENABLE); //ADCʹ��
    ADC_DMACmd(ADC1, ENABLE); //ADC_DMAʹ��
  
	/* 7. ADCУ׼ */
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	//����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����
	 
	/* 8. �������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //��Ϊ�ǳ���ת��ģʽ������ֱ������ת������һֱ����ADCת��
}				  
