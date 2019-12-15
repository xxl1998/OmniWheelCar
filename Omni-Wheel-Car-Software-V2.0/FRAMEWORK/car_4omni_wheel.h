#ifndef __CAR_4OMNI_WHEEL_
#define __CAR_4OMNI_WHEEL_

#define  SpeedK   0.7071f
#define  Wheel_Distance     0.17f   //m
#define  Pi    3.1416f
#define  deg2rad   (Pi/180.0f)
#define  Circumference    (Pi*Wheel_Distance)
#define  SpeedO   (deg2rad*Wheel_Distance/2.0f)

#define  WheelDiameter    0.04f  //40mm
#define  WheelCircumference     (Pi*WheelDiameter)

#define  speed2pwm    1000.0f

#define MAX_X_SPEED        0.2f//  m/s
#define MAX_Y_SPEED        0.2f//  m/s
#define MAX_Omega_SPEED    80.0f//  deg/s

#define  DisArmed  0
#define  Armed     1

typedef struct
{
	unsigned short remoter_thr;
	unsigned short remoter_yaw;
	unsigned short remoter_pit;
	unsigned short remoter_rol;
	float speedx;//  m/s
	float speedy;
	float omega;//  deg/s
	float speedm1stdUnit;//  m/s
	float speedm2stdUnit;
	float speedm3stdUnit;
	float speedm4stdUnit;
	float speedm1;//   rps
	float speedm2;
	float speedm3;
	float speedm4;
	float speedm1PWM;
	float speedm2PWM;
	float speedm3PWM;
	float speedm4PWM;
	unsigned char isArmed;
}CarTypedef;

extern CarTypedef car;

void Speed_Distribution(void);

#endif
