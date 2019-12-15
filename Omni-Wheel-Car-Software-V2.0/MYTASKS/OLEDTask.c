#include "OLEDTask.h"
#include "oled.h"
char stringLine1[30];
char stringLine2[30];
char stringLine3[30];
char stringLine4[30];
char stringLine5[30];
UBaseType_t uxHighWaterMark;
void OLEDTask(void *pvParameters)
{
	static u32 tick = 0;
	u32 lastWakeTime = getSysTickCnt(); 
	static u8 t=0;
	OLED_Init(); //初始化OLED	               
  OLED_ShowString(0,0,(const unsigned char*)"XXL@HIT",12);  
	OLED_ShowString(0,12,(const unsigned char*)"Omni Wheel Car",12);  
	OLED_ShowString(0,24,(const unsigned char*)"-2019/5/1-",12);  
	OLED_ShowString(0,36,(const unsigned char*)"ASCII:",12);  
	OLED_ShowString(64,48,(const unsigned char*)"CODE:",12);  
	OLED_Refresh_Gram();//更新显示到OLED	 
  t=' ';  
//OLED_Test();
//	uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );//用完后去掉198行标志
	while(1)
	{
		vTaskDelayUntil(&lastWakeTime, OLED_Update_DT );
//	 uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); printf("\r\n%ld\r\n",uxHighWaterMark );
			OLED_ShowChar(36,52,t,12,1);//显示ASCII字符	
		OLED_ShowNum(94,52,t,3,12);	//显示ASCII字符的码值    
		OLED_Refresh_Gram();        //更新显示到OLED
		t++;
		if(t>'~')t=' ';  
		delay_ms(500);
		tick++;		
	}
}

