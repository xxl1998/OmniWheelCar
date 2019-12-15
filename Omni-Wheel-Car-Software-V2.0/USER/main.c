#include "includes.h"
#include "LedTask.h"
#include "EncoderTask.h"
#include "BTLinkTask.h"
#include "BLELinkTask.h"
#include "OpenMVLinkTask.h"
#include "OLEDTask.h"
#include "MotorControlTask.h"
#include "adc.h"
/**********
2019-12-3
xxl
�����ٶȼ��㡢PID���ٶȱջ�
M1 ������ �� ���½ӿ� TIM2-CH2 PA1������      ����PC3  PC2 PWM��TIM8CH2
M2 ������ �� ���Ͻӿ� TIM5-CH1 PA0������      ����PB4  PB8 PWM��TIM8CH3
M3 ������ �� ���½ӿ� TIM3-CH1 PA6������      ����PB12 PA4 PWM��TIM8CH1
M4 ������ �� ���Ͻӿ� TIM4-CH1 PB6������      ����PB3  PD2 PWM��TIM8CH4

���˳�����ˣ������PCB��ԭ��ͼ��˿ӡ�Ĳ�һ����
����д���ǲ��Թ��˶���ʽû����ģ�������Ĵ��롢PCB�ȣ�����ȷ����ź��˶�����

����Դ�������̫���ˣ�һȦ220���壬���ת��1תÿ�����ң��ٶȷ�������̫�
��������һ��һ���ģ������翪����Ч����
����������ٶȱջ��ˣ��д��е������ɷ���һ�¾���


�Ҵ�����һ����������QQȺ����ӭ��λ�������۵��ӡ�Ƕ��ʽ������Ⱥ��923703530
����ʱ����һЩ���ϣ����ϵĺ͸��˵ģ�����ӭ�������ؽ�����
����΢�Ź��ںţ�  ��������
CSDN�� ��������1
**********/
void system_init(void)
{
	NVIC_Configuration();
	delay_init();
	ADC10_Init();
	Usart1_Init(8 );
//	TASK_timer_TIM2_Init();
}

TaskHandle_t StartTask_Handler;//������
EventGroupHandle_t EventGroupHandler;	//�¼���־����
int main(void)
{
	system_init();
  xTaskCreate(start_task,"start_task",128,NULL, 1,&StartTask_Handler);   
  vTaskStartScheduler();          //�����������
}

BaseType_t xReturn;
void start_task(void *pvParameters)//��ʼ����������
{
  taskENTER_CRITICAL();           //�����ٽ���
	EventGroupHandler=xEventGroupCreate();	 //�����¼���־��

	xReturn=xTaskCreate( EncoderTask, "ENCODER",150,NULL,1,NULL); printf("%ld\n",xReturn); 
	xReturn=xTaskCreate( LedTask, "LED",150,NULL,1,NULL); printf("%ld\n",xReturn); 
	xReturn=xTaskCreate( BTLinkTask, "BTLINK",500,NULL,4,NULL);printf("%ld\n",xReturn);
  xReturn=xTaskCreate( MotorControlTask, "MOTORCONTROL",500,NULL,6,NULL);printf("%ld\n",xReturn);
	xReturn=xTaskCreate( BLELinkTask, "BLELink",500,NULL,4,NULL); printf("%ld\n",xReturn);
	xReturn=xTaskCreate( OpenMVLinkTask, "OPENMVLINK",500,NULL,4,NULL); printf("%ld\n",xReturn);
	xReturn=xTaskCreate( OLEDTask, "OLED",500,NULL,4,NULL); printf("%ld\n",xReturn);
	
  vTaskDelete(StartTask_Handler); //ɾ����ʼ����
  taskEXIT_CRITICAL();            //�˳��ٽ���
}
