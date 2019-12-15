#ifndef _COMM_APP_H
#define _COMM_APP_H

#define MSP_SET_THRO  1
#define MSP_SET_YAW		2
#define MSP_SET_PITCH	3
#define MSP_SET_ROLL	4
#define MSP_ARM_IT		5
#define MSP_DISARM_IT	6
#define MSP_SET_4CON	7
#define MSP_SETOFF		8
#define MSP_LAND_DOWN 9
#define MSP_HOLD_ALT 	10
#define MSP_STOP_HOLD_ALT 11
#define MSP_HEAD_FREE 12
#define MSP_STOP_HEAD_FREE	13
#define MSP_POS_HOLD 14
#define MSP_STOP_POS_HOLD 15
#define MSP_FLY_STATE	16
#define MSP_ACC_CALI	205

#define APP_YAW_DB	 70 //dead band 
#define APP_PR_DB		 50

typedef unsigned char  uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef  short int16_t;


#define CUT_DB(x,mid,DB) {if(fabs(x-mid)<DB) x=mid; \
											else if(x-mid>0) x=x-DB;\
											else if(x-mid<0) x=x+DB;}

extern uint16_t App_remoter_Data[4];
											
void CommApp(unsigned char ch);
void CommAppUpload(void);
void CommAppDeal(void);
void uart16chk(int16_t a);
void uart32chk(uint32_t a);
void uart8chk(uint8_t _x);
											
#endif

