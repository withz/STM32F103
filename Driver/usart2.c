#include "usart2.h"

const u32 usart2_rx_buf_size = 256;
const u32 usart2_tx_buf_size = 256;

u8 usart2_rx_buffer[usart2_rx_buf_size];
u8 usart2_tx_buffer[usart2_tx_buf_size];

void USART2_OnRecv(s32 len, u8* c);
void USART2_OnSend(void);

void (*USART2_OnRecvHandler)(s32 len, u8* c) = USART2_OnRecv;
void (*USART2_OnSendHandler)() = USART2_OnSend;

void USART2_Configuration(void)		
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_2;  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);

    USART_DeInit(USART2);
    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART2,&usart);

    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)usart2_rx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = usart2_rx_buf_size;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6,&dma);

    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)usart2_tx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralDST;
    dma.DMA_BufferSize = usart2_tx_buf_size;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7,&dma);
 
    nvic.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
    DMA_Cmd(DMA1_Channel6, ENABLE);
    DMA_Cmd(DMA1_Channel7, DISABLE);
    
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
    
    USART_Cmd(USART2,ENABLE);
}

void USART2_SendChar(u8 b)
{
	unsigned short wait = 60000;
	while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET){
        if(wait < 10) return;
        wait--;
	}
	USART_SendData(USART2,b);
}

void USART2_SendWords(u8* c, s32 len)
{
	unsigned short wait = 60000;
	while(len--){
        wait = 60000;
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET){
            if(wait < 10) return;
            wait--;
        }
		USART_SendData(USART2,*c);
		c++;
	}
}


void USART2_DMASendWords(u8 *c, int len)
{
	u32 i;
    u32 n=0;
    u32 part=len/usart2_tx_buf_size;
    
    for(n=0;n<part;n++){
        while(DMA_GetFlagStatus(DMA1_FLAG_TC7)!=RESET);
        for(i=0;i<usart2_tx_buf_size;i++){
            usart2_tx_buffer[i]=c[i+n*usart2_tx_buf_size];
        }
        DMA_SetCurrDataCounter(DMA1_Channel7, usart2_tx_buf_size);
        DMA_Cmd(DMA1_Channel7, ENABLE);
        len=len-usart2_tx_buf_size;
    }
    while(DMA_GetFlagStatus(DMA1_FLAG_TC7)!=RESET);
    for(i=0;i<len;i++){
        usart2_tx_buffer[i]=c[i+n*usart2_tx_buf_size];
    }
    DMA_SetCurrDataCounter(DMA1_Channel7, len);
    DMA_Cmd(DMA1_Channel7, ENABLE);
}

void USART2_OnRecv(s32 len, u8* c)
{
    
}

void USART2_OnSend()
{
    
}


void USART2_IRQHandler()
{
    u8 rec=0;
    s32 len=0;

    if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET){
        DMA_Cmd(DMA1_Channel6, DISABLE);

		DMA_ClearFlag(DMA1_FLAG_TC6);
		len = usart2_rx_buf_size - DMA_GetCurrDataCounter(DMA1_Channel6);
		if(len<0){
			len=0;
		}
        
		DMA_SetCurrDataCounter(DMA1_Channel6, usart2_rx_buf_size);
		DMA_Cmd(DMA1_Channel6, ENABLE);
		
		(*USART2_OnRecvHandler)(len, usart2_rx_buffer);
		
		rec=USART_ReceiveData(USART2);
		rec=rec;
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
	}
}

void DMA1_Channel7_IRQHandler() 
{
    if(DMA_GetITStatus(DMA1_IT_TC7) == SET){
        DMA_Cmd(DMA1_Channel7, DISABLE);
        (*USART2_OnSendHandler)();
        DMA_ClearFlag(DMA1_FLAG_TC7);
        DMA_ClearITPendingBit(DMA1_IT_TC7);
    }
}

