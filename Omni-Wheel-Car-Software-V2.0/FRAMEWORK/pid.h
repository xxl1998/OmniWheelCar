#ifndef __PID_H
#define __PID_H

typedef struct
{
	float Des;
	float FB;
	float E;
	float Kp;
	float Ki;
	float Kd;
	float PreE;
	float SumE;
	float Up;
	float Ui;
	float Ud;
	float U;
	float Upmax;
	float Uimax;
	float Udmax;
	float Umax;
}  	PIDTypeDef;

typedef struct 
{
	PIDTypeDef  M1Pid;
	PIDTypeDef  M2Pid;
	PIDTypeDef  M3Pid;
	PIDTypeDef  M4Pid;
}WheelPidTypeDef;

void PidUpdate(PIDTypeDef* pPID);

extern WheelPidTypeDef  WheelPid;

#endif
