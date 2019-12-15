#include "EncoderTask.h"
#include "sys.h" 
#include "delay.h"
#include "exti.h"
#include "tim4_capture.h"
#include "pid.h"


void EncoderTask(void *pvParameters)
{
	static u32 tick = 0;
	u32 lastWakeTime = getSysTickCnt();
	EXTIX_Init();
	TIM_4_Cap_Init(3,0);
	while(1)
	{
		vTaskDelayUntil(&lastWakeTime, Encoder_Scan_DT );
		WheelPid.M1Pid.FB = encoder_cnt[1]/220.0f /(Encoder_Scan_DT/1000.0f);
		WheelPid.M2Pid.FB = encoder_cnt[2]/220.0f /(Encoder_Scan_DT/1000.0f);
		WheelPid.M3Pid.FB = encoder_cnt[3]/220.0f /(Encoder_Scan_DT/1000.0f);
		WheelPid.M4Pid.FB = encoder_cnt[4]/220.0f /(Encoder_Scan_DT/1000.0f);
		
		encoder_cnt[1] = 0;
		encoder_cnt[2] = 0;
		encoder_cnt[3] = 0;
		encoder_cnt[4] = 0;
		
		tick++;
	}
}
