#include "car_4omni_wheel.h"
#include "pid.h"

CarTypedef car;

void Speed_Distribution(void)
{
	//anticircular +     forward y+   right x+     anticlock omega+
	car.speedx = ( car.remoter_rol -3000 )/1000.0f * MAX_X_SPEED;
	car.speedy = ( car.remoter_pit -3000 )/1000.0f * MAX_Y_SPEED;
	car.omega  = -( car.remoter_yaw -3000 )/1000.0f * MAX_Omega_SPEED;
	
	car.speedm1stdUnit= car.speedx *SpeedK  + car.speedy *SpeedK - car.omega *SpeedO;
	car.speedm2stdUnit= car.speedx *SpeedK  - car.speedy *SpeedK - car.omega *SpeedO;
	car.speedm3stdUnit= -car.speedx *SpeedK + car.speedy *SpeedK - car.omega *SpeedO;
	car.speedm4stdUnit=	-car.speedx *SpeedK - car.speedy *SpeedK - car.omega *SpeedO;
	
	car.speedm1 = car.speedm1stdUnit / WheelCircumference;
	car.speedm2 = car.speedm2stdUnit / WheelCircumference;
	car.speedm3 = car.speedm3stdUnit / WheelCircumference;
	car.speedm4 = car.speedm4stdUnit / WheelCircumference;
	
	WheelPid.M1Pid.Des = car.speedm1;
	WheelPid.M2Pid.Des = car.speedm2;
	WheelPid.M3Pid.Des = car.speedm3;
	WheelPid.M4Pid.Des = car.speedm4;
	
	PidUpdate(&WheelPid.M1Pid);
	PidUpdate(&WheelPid.M2Pid);
	PidUpdate(&WheelPid.M3Pid);
	PidUpdate(&WheelPid.M4Pid);
	
	car.speedm1PWM = car.speedm1 * speed2pwm ;//+ WheelPid.M1Pid.U;
	car.speedm2PWM = car.speedm2 * speed2pwm ;//+ WheelPid.M2Pid.U;
	car.speedm3PWM = car.speedm3 * speed2pwm ;//+ WheelPid.M3Pid.U;
	car.speedm4PWM = car.speedm4 * speed2pwm ;//+ WheelPid.M4Pid.U;
}
