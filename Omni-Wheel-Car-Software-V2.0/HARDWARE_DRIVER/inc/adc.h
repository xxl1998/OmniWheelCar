#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
#include "stm32f10x_adc.h" 
#include "stm32f10x_gpio.h" 
#include "stm32f10x_dma.h" 


extern __IO uint16_t ADC_RawValue[10];
extern __IO float ConvertedVoltage;
extern __IO unsigned int ADC_FilteredRaw;

void ADC10_Init(void); 				//ADC通道初始化
void ADC_Get_Voltage(void);

#endif 
