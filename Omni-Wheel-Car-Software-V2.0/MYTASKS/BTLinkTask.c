#include "BTLinkTask.h"
#include "usart1.h"


void BTLinkTask(void *pvParameters)
{
	while(1)
	{
			vTaskDelay(10);//事件标志组还没有，就延时10ms，也就是10个时钟节拍
	}
}
