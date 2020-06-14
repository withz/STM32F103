#ifndef _LORA_H_
#define _LORA_H_

#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "usart2.h"




typedef struct LORA_Info_s {
    u8 Channel;
    u8 WorkMode;
    u8 Busy;
    u8 WaitRecv;
    u8 SendMode;
    u16 Address;
    
} LORA_Info;

extern LORA_Info LoraInfo;
extern void (*LORA_OnRecvHandler)(s32 len, u8 *buf);

void LORA_Configuration(void);
void LORA_ChangeWorkMode(u8 n);
void LORA_AskInfo(void);
void LORA_Reset(void);

void LORA_SetConfig(LORA_Info *m);
LORA_Info *LORA_GetConfig(void);

void LORA_Send(u8 *c, u8 len);
void LORA_SendToNode(u8 ch, u16 addr, u8 *c, u8 len);
void LORA_Broadcast(u8 ch, u8 *c, u8 len);
u8 LORA_WaitRecv(void);

#endif
