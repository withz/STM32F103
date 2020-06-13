#ifndef USART3_H
#define USART3_H

#include "stm32f10x.h"


void USART3_Configuration(void);
void USART3_SendChar(u8 b);
void USART3_SendWords(u8* c, s32 len);
void USART3_DMASendWords(u8 *c, int len);

extern void (*USART3_OnRecvHandler)(s32 len, u8* c);
extern void (*USART3_OnSendHandler)();

#endif
