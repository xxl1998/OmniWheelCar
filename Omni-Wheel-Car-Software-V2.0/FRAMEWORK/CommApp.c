#include "CommApp.h"
#include "usart2.h"
#include "includes.h"
#include "car_4omni_wheel.h"

uint16_t App_remoter_Data[4]= {1500,1500,1500,1500};
//#define CONSTRAIN(x,min,max) {if(x<min) x=min; if(x>max) x=max;}  CONSTRAIN(rcData[THROTTLE],1000,2000);

void CommAppDeal(void)
{
	unsigned int i;
	for(i=0;i<USART2_REC_CNT;i++)CommApp(USART2_RX_BUF[i]);
}

uint8_t ComAPPTxbuf[50];
uint8_t TxChecksum;
uint8_t TxBufP;
void CommAppUpload(void)
{
	static unsigned char cnt;
	unsigned char i;
		TxBufP=0;
    uart8chk('$');
    uart8chk('M');
    uart8chk('>');
    TxChecksum = 0;
    uart8chk(12+2);
    uart8chk(MSP_FLY_STATE);
    uart16chk((int16_t)(1.0f * 10));//roll
    uart16chk((int16_t)(2.0f * 10));//pitch
    uart16chk((int16_t)(3.0f * 10));//yaw
    uart32chk((int32_t)(4.0f * 100));	//altitude   -nav.z
    uart16chk((int16_t)(5.0f * 100));//baterry voltage       Battery.BatteryVal
    uart16chk((int16_t)(6.0f * 1000));//vz   -nav.vz
    uart8chk(TxChecksum);
 //   USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	for(i=0;i<TxBufP;i++)USART2_TX_BUF[i]=ComAPPTxbuf[i];
	if(cnt>=2)
	{
		USART2_DMA_TX_Enable(TxBufP);
		cnt=0;
	}
	cnt++;
}

#define MAX_LEN 32
volatile uint8_t UdataBuf[MAX_LEN];
void CommAppCmdProcess(void)
{
    //process
    switch(UdataBuf[4])//MSP_SET_4CON
    {
    case MSP_SET_4CON:	//UdataBuf
        App_remoter_Data[0]=UdataBuf[6]<<8 | UdataBuf[5];//thr
        App_remoter_Data[1]=UdataBuf[8]<<8 | UdataBuf[7];//yaw
        App_remoter_Data[2]=UdataBuf[10]<<8 | UdataBuf[9];//pit
        App_remoter_Data[3]=UdataBuf[12]<<8 | UdataBuf[11];//roll
        break;
    case MSP_ARM_IT://arm，上锁
			car.isArmed = Armed;
        break;
    case MSP_DISARM_IT://disarm，解锁
			car.isArmed = DisArmed;
        break;
    case MSP_ACC_CALI:    //    imuCaliFlag=1;
        break;
    case MSP_HEAD_FREE:
        break;
    case MSP_STOP_HEAD_FREE:
        break;
    case MSP_LAND_DOWN:		//自动降落
        break;
    }
}

static uint8_t bufP=0;//
static uint8_t checksum=0;
static uint8_t validDataLen=0;
void CommApp(uint8_t ch)
{
    UdataBuf[bufP]= ch;
    if(bufP<3)
    {
        switch(bufP)
        {
        case 0:
            if(UdataBuf[bufP]=='$')
                bufP++;
            break;
        case 1:
            if(UdataBuf[bufP]=='M')
                bufP++;
            else
                bufP=0;
            break;
        case 2:
            if(UdataBuf[bufP]=='<')
                bufP++;
            else
                bufP=0;
            break;
        }
    }
    else	//valid data
    {
        if(bufP==3)		//len
        {
            checksum=0;
            validDataLen=UdataBuf[bufP];
        }

        bufP++;
        if(bufP >= validDataLen + 6)	// rec over. process. tobe placed in 50Hz loop
        {
            //chksum
            if(UdataBuf[bufP-1]==checksum)
            {
							if(UdataBuf[4] == MSP_FLY_STATE){	/*flyLogApp=1;*/		}            
               CommAppCmdProcess();		//could be place to main           
            }
            bufP=0;
        }
        else        checksum^=UdataBuf[bufP-1];
    }
}

static  void uart8chk(uint8_t _x)
{
	ComAPPTxbuf[TxBufP++] = _x;
  TxChecksum ^= _x;
}

static  void uart32chk(uint32_t a)
{
    static uint8_t t;
    t = a;
		ComAPPTxbuf[TxBufP++] = t;
    TxChecksum ^= t;
    t = a >> 8;
    ComAPPTxbuf[TxBufP++] = t;
    TxChecksum ^= t;
    t = a >> 16;
    ComAPPTxbuf[TxBufP++] = t;
    TxChecksum ^= t;
    t = a >> 24;
    ComAPPTxbuf[TxBufP++] = t;
    TxChecksum ^= t;
}

static void uart16chk(int16_t a)
{
    static uint8_t t;
    t = a;
    ComAPPTxbuf[TxBufP++] = t;
    TxChecksum ^= t;
    t = a >> 8 & 0xff;
    ComAPPTxbuf[TxBufP++] = t;
    TxChecksum ^= t;
}
