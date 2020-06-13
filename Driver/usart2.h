#ifndef USART2_H
#define USART2_H

#include "stm32f10x.h"


void USART2_Configuration(void);
void USART2_SendChar(u8 b);
void USART2_SendWords(u8* c, s32 len);
void USART2_DMASendWords(u8 *c, int len);

extern void (*USART2_OnRecvHandler)(s32 len, u8* c);
extern void (*USART2_OnSendHandler)();

#endif
