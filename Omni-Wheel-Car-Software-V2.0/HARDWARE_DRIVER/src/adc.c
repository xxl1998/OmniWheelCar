#include "adc.h"
#include "delay.h"		 
#include "stdio.h"

__IO uint16_t ADC_RawValue[10]; //用于存储DMA传输过来的缓存数组(12通道，每通道采10个值)
__IO float  ConvertedVoltage; //存放平均滤波后的ADC值
__IO unsigned int ADC_FilteredRaw;

void ADC_Get_Voltage(void)
{
	uint8_t i=0;
	ADC_FilteredRaw=0;
	//采10个值，进行简单的平均滤波
	for(i=0; i<10; i++)
	{
		ADC_FilteredRaw += (ADC_RawValue[i] & 0x0fff); //确保取得低12位有效的ADC数据
  }
	ADC_FilteredRaw /=10;
		
	//测试打印ADC值对应的电压值
	ConvertedVoltage= ADC_FilteredRaw*3.32f/4096.0f;
	//printf("ADC = raw:%d --- %fV\r\n", ADC_FilteredRaw, ConvertedVoltage);
}

//初始化ADC															   
void ADC10_Init(void)
{    
  ADC_InitTypeDef ADC_InitStructure;       
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO端口设置
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/* 1. ADC时钟配置 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //打开ADC时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); //配置时钟(12MHz),F1不超过14MHZ
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//打开DMA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//打开GPIO时钟
	
	/* 2. ADC的GPIO初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//GPIOC
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
	/* 3. ADC参数配置 */
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //开启持续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //无外部触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1和ADC2独立模式
	ADC_InitStructure.ADC_NbrOfChannel = 1; //通道数
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //开启持续扫描模式 
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器 
	
	/* 4. ADC通道配置 */
	//这里配置的ADCCLK=12MHz,55.5周期的采样时间
	//则TCONV=55.5+12.5=68个周期=68/12MHz=5.67us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10,11, ADC_SampleTime_239Cycles5);
    
	/* 5. DMA初始化 */
	DMA_DeInit(DMA1_Channel1); //先复位对应DMA通道的寄存器值
	
	//相应的DMA配置
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //数据源地址  ADC数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_RawValue; //内存中存储DMA传输过来的数据基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = 10; //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //不使用内存到内存
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设寄存器地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址要增加
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA外设数据宽度 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //内存数据为半字 16位（ADC精度为12位）
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMA工作在循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA优先级中
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //DMA初始化
	
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA
	
    /* 6. 使能ADC */
    ADC_Cmd(ADC1, ENABLE); //ADC使能
    ADC_DMACmd(ADC1, ENABLE); //ADC_DMA使能
  
	/* 7. ADC校准 */
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	//开启AD校准
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束
	 
	/* 8. 软件启动ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //因为是持续转换模式，所以直接启动转换，将一直进行ADC转换
}				  
