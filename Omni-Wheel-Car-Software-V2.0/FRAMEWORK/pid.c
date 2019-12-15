#include "pid.h"
//  Des FB E   Kp  Ki  Kd PreE SumE Up Ui Ud U  Upmax Uimax Udmax Umax
WheelPidTypeDef  WheelPid=
{
	{  0, 0, 0, 1000 ,  1, 1 , 0,   0,  0,  0, 0,0,  100,  10,   10,   100},
	{  0, 0, 0, 1000 ,  1, 1 , 0,   0,  0,  0, 0,0,  100,  10,   10,   100},
	{  0, 0, 0, 1000 ,  1, 1 , 0,   0,  0,  0, 0,0,  100,  10,   10,   100},
	{  0, 0, 0, 1000 ,  1, 1 , 0,   0,  0,  0, 0,0,  100,  10,   10,   100}
};


void PidUpdate(PIDTypeDef* pPID)
{
	if( (pPID->Des>0 && pPID->FB<0) || (pPID->Des<0 && pPID->FB>0) )
	{
		pPID->FB = -pPID->FB;
		pPID->SumE = 0;
	}
	pPID->E = pPID->Des - pPID->FB;
	pPID->SumE += pPID->E;
	
	pPID->Up = pPID->Kp * pPID->E;
	pPID->Ui = pPID->Ki * pPID->SumE;
	pPID->Ud = pPID->Kd * (pPID->E - pPID->PreE);
	
	pPID->U = pPID->Up ;// + pPID->Ui + pPID->Ud;
	
	if( pPID->U > pPID->Umax)pPID->U = pPID->Umax;
	if( pPID->U < -pPID->Umax)pPID->U = -pPID->Umax;
	
	pPID->PreE = pPID->E;
	
	if(pPID->Des == 0 ) pPID->U = 0;
}
