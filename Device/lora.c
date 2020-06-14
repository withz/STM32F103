#include "lora.h"


#define LORA_M0_PORT		MA
#define LORA_M1_PORT		MA
#define LORA_AUX_PORT		MA

#define LORA_M0_PIN		    M1
#define LORA_M1_PIN			M2
#define LORA_AUX_PIN		M3


#define LORA_USART_Init		           USART2_Configuration
#define LORA_BaseSend                  USART2_DMASendWords    
#define LORA_BaseOnRecvHandler         USART2_OnRecvHandler 
#define LORA_BaseOnSendFinishHandler   USART2_OnSendHandler

void LORA_BaseOnSendFinish(void);
void LORA_BaseOnRecv(s32 len, u8 *buf);
void LORA_OnRecv(s32 len, u8 *buf);
void LORA_SendData(u32 len, u8 *buf);
u8 LORA_WaitSend(void);


LORA_Info LoraInfo;
void (*LORA_OnRecvHandler)(s32 len, u8 *buf) = LORA_OnRecv;



void LORA_Configuration(void)
{
	MG_RCC(LORA_M0_PORT);
    MG_RCC(LORA_M1_PORT);
    MG_RCC(LORA_AUX_PORT);
    
    MG_Input(LORA_AUX_PORT,LORA_AUX_PIN,1);
    MG_Output(LORA_M0_PORT,LORA_M0_PIN);
    MG_Output(LORA_M1_PORT,LORA_M1_PIN);
    
    LORA_BaseOnSendFinishHandler = LORA_BaseOnSendFinish;
    LORA_BaseOnRecvHandler = LORA_BaseOnRecv;
	LORA_USART_Init();
	
	LoraInfo.Channel=0X17;
	LoraInfo.Address=0X1234;
    LoraInfo.Busy=0;
    LoraInfo.WaitRecv=0;
    LoraInfo.SendMode=0;
    LORA_ChangeWorkMode(0);
    LORA_SetConfig(&LoraInfo);
}

void LORA_ChangeWorkMode(u8 n)
{
	if(n>3) n=3;
    LoraInfo.WorkMode=n;
	switch(n){
		case 0:  // Normal
            MG_Set(LORA_M0_PORT, LORA_M0_PIN,0);
            MG_Set(LORA_M1_PORT, LORA_M1_PIN,0);
			break;
		case 1:  // Save Power Mode
            MG_Set(LORA_M0_PORT, LORA_M0_PIN,1);
            MG_Set(LORA_M1_PORT, LORA_M1_PIN,0);
			break;
		case 2:  // Awake Mode
			MG_Set(LORA_M0_PORT, LORA_M0_PIN,0);
            MG_Set(LORA_M1_PORT, LORA_M1_PIN,1);
			break;
		case 3:  // Sleep Mode / Config Mode
			MG_Set(LORA_M0_PORT, LORA_M0_PIN,1);
            MG_Set(LORA_M1_PORT, LORA_M1_PIN,1);
			break;
	}
	delayms(10);
}

void LORA_AskInfo(void)
{
    u8 buf[3]={0XC3,0XC3,0XC3};
	LORA_SendData(3,buf);
}

void LORA_Reset(void)
{
	u8 buf[3]={0XC4,0XC4,0XC4};
	LORA_SendData(3,buf);
}

void LORA_SetConfig(LORA_Info *m)
{
    u8 buf[10]={0XC2, ((m->Address)&0xFF00)>>8, ((m->Address)&0xFF), 0x1A, m->Channel, 0x44|(m->SendMode<<7)};
    LORA_SendData(6,buf);
}

LORA_Info *LORA_GetConfig(void)
{
    u8 buf[3]={0XC1, 0XC1, 0XC1};
    LORA_SendData(3,buf);
    LoraInfo.WaitRecv=10;
    return &LoraInfo;
}




void LORA_Send(u8 *c, u8 len)
{
	LORA_SendData(len, c);
}

void LORA_SendToNode(u8 ch, u16 addr, u8 *c, u8 len)
{
    u8 buf[4] = {(addr>>7)&0XFF, addr&0XFF, ch, len};
    LORA_SendData(4, buf);
	LORA_SendData(len, c);
}

void LORA_Broadcast(u8 ch, u8 *c, u8 len)
{
	u8 buf[4] = {0XFF, 0XFF, ch, len};
    LORA_SendData(4, buf);
	LORA_SendData(len, c);
}

u8 LORA_WaitRecv(void)
{
    u8 re=0;
    u32 wait=1000;
    while(LoraInfo.WaitRecv){
        if(wait<10) {
            re=1;
            break;
        }
        delayms(1);
        wait--;
    }
	return re;
}



void LORA_BaseOnSendFinish(void)
{
    LoraInfo.Busy=0;
}

void LORA_BaseOnRecv(s32 len, u8 *buf)
{
    if(LoraInfo.WorkMode==3){
        if(LoraInfo.WaitRecv==10){ // get config
            LoraInfo.Address=(buf[1]<<8)+buf[2];
            LoraInfo.Channel=buf[4];
            LoraInfo.SendMode=(buf[5]&0x80)>>7;
        }
    }else{
        LORA_OnRecvHandler(len, buf);
    }
    LoraInfo.WaitRecv=0;
}

void LORA_OnRecv(s32 len, u8 *buf)
{
    
}

u8 LORA_WaitSend(void)
{
    u8 re=0;
    u32 wait=20000;
    while(LoraInfo.Busy){
        if(wait<10) {
            re=1;
            break;
        }
        wait--;
    }
    wait=20000;
    while(MG_Get(LORA_AUX_PORT, LORA_AUX_PIN)==RESET){
        if(wait<10) {
            re=1;
            break;
        }
        wait--;
    }
	return re;
}

void LORA_SendData(u32 len, u8 *buf)
{
    while(LORA_WaitSend());
    LORA_BaseSend(buf, len);
    LoraInfo.Busy=1;
    if(LoraInfo.WorkMode!=3){
        LoraInfo.WaitRecv=1;
    }
}
