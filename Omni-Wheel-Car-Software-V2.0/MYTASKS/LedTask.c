#include "LedTask.h"
#include "tim1_npwm.h"

void LedTask(void *pvParameters)
{
	TIM1_NPWM_OUTPUT_Init();
	while(1)
	{
		vTaskDelay(LED_DT ); 	
		RGBLED_G-=2;
		RGBLED_B-=2;
		RGBLED_R-=2;
		if(RGBLED_G<=0)RGBLED_G=1000;
		if(RGBLED_R<=0)RGBLED_R=1000;
		if(RGBLED_B<=0)RGBLED_B=1000;
	}
}
