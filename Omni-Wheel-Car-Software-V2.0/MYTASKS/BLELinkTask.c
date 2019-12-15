#include "BLELinkTask.h"
#include "usart2.h"
#include "CommApp.h"
#include "car_4omni_wheel.h"

void BLELinkTask(void *pvParameters)
{
	EventBits_t uxBits;
	const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
	Usart2_Init(5);//HC-08
	
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			uxBits = xEventGroupWaitBits(
								EventGroupHandler,	//事件标志组句柄
								got_USART2_DATA_BIT,
								pdTRUE,        /* BITs should be cleared before returning. */
								pdTRUE,       /* 等待4个通道都捕获 */
								xTicksToWait );/* Wait a maximum of 100ms for either bit to be set. */
			if( ( uxBits & (got_USART2_DATA_BIT) ) != 0 )
			{
				CommAppDeal();
				CommAppUpload();
				car.remoter_thr = App_remoter_Data[0]*2;
				car.remoter_yaw = App_remoter_Data[1]*2;
				car.remoter_pit = App_remoter_Data[2]*2;
				car.remoter_rol = App_remoter_Data[3]*2;	
			}
			else//超时
			{
				car.isArmed = DisArmed;
				car.remoter_thr = 3000;
				car.remoter_yaw = 3000;
				car.remoter_pit = 3000;
				car.remoter_rol = 3000;	
			}
		}
		else
		{
			vTaskDelay(10);//事件标志组还没有，就延时10ms，也就是10个时钟节拍
		}
	}
}
