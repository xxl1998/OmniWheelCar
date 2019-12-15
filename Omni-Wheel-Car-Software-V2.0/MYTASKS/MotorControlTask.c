#include "MotorControlTask.h"
#include "adc.h"
#include "motors.h"
#include "car_4omni_wheel.h"

void MotorControlTask(void *pvParameters)
{
	static u32 tick = 0;
	u32 lastWakeTime = getSysTickCnt(); 	
	Motors_Init();
	while(1)
	{
		vTaskDelayUntil(&lastWakeTime, Ctrl_DT );
		ADC_Get_Voltage();
		
		Speed_Distribution();
		
		if(car.isArmed == Armed)
		{
			if(car.speedm1PWM>=0)Set_Motor_PWM(M1,Anticlockwise,car.speedm1PWM);
			else Set_Motor_PWM(M1,Clockwise,-car.speedm1PWM);
			
			if(car.speedm2PWM>=0)Set_Motor_PWM(M2,Anticlockwise,car.speedm2PWM);
			else Set_Motor_PWM(M2,Clockwise,-car.speedm2PWM);
			
			if(car.speedm3PWM>=0)Set_Motor_PWM(M3,Anticlockwise,car.speedm3PWM);
			else Set_Motor_PWM(M3,Clockwise,-car.speedm3PWM);
			
			if(car.speedm4PWM>=0)Set_Motor_PWM(M4,Anticlockwise,car.speedm4PWM);
			else Set_Motor_PWM(M4,Clockwise,-car.speedm4PWM);
		}


		tick++;		
	}
}
