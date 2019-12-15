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
加入速度计算、PID、速度闭环
M1 左上轮 接 左下接口 TIM2-CH2 PA1编码器      方向PC3  PC2 PWM：TIM8CH2
M2 右上轮 接 左上接口 TIM5-CH1 PA0编码器      方向PB4  PB8 PWM：TIM8CH3
M3 左下轮 接 右下接口 TIM3-CH1 PA6编码器      方向PB12 PA4 PWM：TIM8CH1
M4 右下轮 接 右上接口 TIM4-CH1 PB6编码器      方向PB3  PD2 PWM：TIM8CH4

电机顺序乱了，代码和PCB、原理图、丝印的不一样，
上面写的是测试过运动形式没问题的，如果更改代码、PCB等，自行确定编号和运动分配

电机自带编码器太挫了，一圈220脉冲，最大转速1转每秒左右，速度反馈数据太差，
控制起来一卡一卡的，还不如开环的效果，
放弃做电机速度闭环了，有大佬调出来可分享一下经验


我创建了一个技术交流QQ群，欢迎各位加入讨论电子、嵌入式技术。群号923703530
不定时分享一些资料（网上的和个人的），欢迎过来下载交流。
个人微信公众号：  飞觞醉月
CSDN： 飞觞醉月1
**********/
void system_init(void)
{
	NVIC_Configuration();
	delay_init();
	ADC10_Init();
	Usart1_Init(8 );
//	TASK_timer_TIM2_Init();
}

TaskHandle_t StartTask_Handler;//任务句柄
EventGroupHandle_t EventGroupHandler;	//事件标志组句柄
int main(void)
{
	system_init();
  xTaskCreate(start_task,"start_task",128,NULL, 1,&StartTask_Handler);   
  vTaskStartScheduler();          //开启任务调度
}

BaseType_t xReturn;
void start_task(void *pvParameters)//开始任务任务函数
{
  taskENTER_CRITICAL();           //进入临界区
	EventGroupHandler=xEventGroupCreate();	 //创建事件标志组

	xReturn=xTaskCreate( EncoderTask, "ENCODER",150,NULL,1,NULL); printf("%ld\n",xReturn); 
	xReturn=xTaskCreate( LedTask, "LED",150,NULL,1,NULL); printf("%ld\n",xReturn); 
	xReturn=xTaskCreate( BTLinkTask, "BTLINK",500,NULL,4,NULL);printf("%ld\n",xReturn);
  xReturn=xTaskCreate( MotorControlTask, "MOTORCONTROL",500,NULL,6,NULL);printf("%ld\n",xReturn);
	xReturn=xTaskCreate( BLELinkTask, "BLELink",500,NULL,4,NULL); printf("%ld\n",xReturn);
	xReturn=xTaskCreate( OpenMVLinkTask, "OPENMVLINK",500,NULL,4,NULL); printf("%ld\n",xReturn);
	xReturn=xTaskCreate( OLEDTask, "OLED",500,NULL,4,NULL); printf("%ld\n",xReturn);
	
  vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}
