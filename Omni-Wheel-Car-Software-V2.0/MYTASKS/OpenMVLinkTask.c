#include "OpenMVLinkTask.h"
#include "string.h"
#include "delay.h"

void OpenMVLinkTask(void *pvParameters)
{

	while(1)
	{
		vTaskDelay(Openmv_DT);
	}
}/*获取数据*/
