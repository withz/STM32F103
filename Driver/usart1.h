#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"


void USART1_Configuration(void);
void USART1_SendChar(u8 b);
void USART1_SendWords(u8* c, s32 len);
void USART1_DMASendWords(u8 *c, int len);

extern void (*USART1_OnRecvHandler)(s32 len, u8* c);
extern void (*USART1_OnSendHandler)();

#endif
