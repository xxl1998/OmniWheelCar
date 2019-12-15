#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

/********宏定义区*********/

#define RATE_5_HZ		5
#define RATE_10_HZ		10
#define RATE_25_HZ		25
#define RATE_50_HZ		50
#define RATE_100_HZ		100
#define RATE_200_HZ 	200
#define RATE_250_HZ 	250
#define RATE_500_HZ 	500
#define RATE_1000_HZ 	1000

#define MAIN_LOOP_RATE 	RATE_50_HZ
#define MAIN_LOOP_DT	(u32)(1000/MAIN_LOOP_RATE)	/*单位ms*/

#define RATE_DO_EXECUTE(RATE_HZ, TICK) ((TICK % (MAIN_LOOP_RATE / RATE_HZ)) == 0)


#define got_USART1_DATA_BIT	  ( 1 << 0 )
#define got_USART2_DATA_BIT 	( 1 << 1 )
#define got_USART3_DATA_BIT	  ( 1 << 2 )
#define got_USART4_DATA_BIT	  ( 1 << 3 )

#define Encoder_Scan_DT           200    //ms
#define Ctrl_DT   100
#define LED_DT  20
#define Openmv_DT      200
#define OLED_Update_DT    40

void start_task(void *pvParameters);//任务函数

#endif
