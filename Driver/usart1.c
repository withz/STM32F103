#include "usart1.h"

const u32 usart1_rx_buf_size = 256;
const u32 usart1_tx_buf_size = 256;

u8 usart1_rx_buffer[usart1_rx_buf_size];
u8 usart1_tx_buffer[usart1_tx_buf_size];

void USART1_OnRecv(s32 len, u8* c);
void USART1_OnSend(void);

void (*USART1_OnRecvHandler)(s32 len, u8* c) = USART1_OnRecv;
void (*USART1_OnSendHandler)() = USART1_OnSend;

void USART1_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_9;  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);

    USART_DeInit(USART1);
    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART1,&usart);
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)usart1_rx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = usart1_rx_buf_size;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5,&dma);
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)usart1_tx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralDST;
    dma.DMA_BufferSize = usart1_tx_buf_size;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4,&dma);

    nvic.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
    
    DMA_Cmd(DMA1_Channel5, ENABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);
    
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
    
    USART_Cmd(USART1,ENABLE);
}

void USART1_SendChar(u8 b)
{
	unsigned short wait = 60000;
    while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET){
        if(wait < 10) return;
        wait--;
	}
	USART_SendData(USART1,b);
}

void USART1_SendWords(u8* c, s32 len)
{
	unsigned short wait = 60000;
	while(len--){
        wait = 60000;
		while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET){
            if(wait < 10) return;
            wait--;
        }
		USART_SendData(USART1,*c);
		c++;
	}
}

void USART1_DMASendWords(u8 *c, int len)
{
	u32 i;
    u32 n=0;
    u32 part=len/usart1_tx_buf_size;
    
    for(n=0;n<part;n++){
        while(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET);
        for(i=0;i<usart1_tx_buf_size;i++){
            usart1_tx_buffer[i]=c[i+n*usart1_tx_buf_size];
        }
        DMA_SetCurrDataCounter(DMA1_Channel4, usart1_tx_buf_size);
        DMA_Cmd(DMA1_Channel4, ENABLE);
        len=len-usart1_tx_buf_size;
    }
    while(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET);
    for(i=0;i<len;i++){
        usart1_tx_buffer[i]=c[i+n*usart1_tx_buf_size];
    }
    DMA_SetCurrDataCounter(DMA1_Channel4, len);
    DMA_Cmd(DMA1_Channel4, ENABLE);
}

void USART1_OnRecv(s32 len, u8* c)
{
    
}

void USART1_OnSend()
{
    
}


void USART1_IRQHandler()
{
    u8 rec=0;
    s32 len=0;

    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET){
        DMA_Cmd(DMA1_Channel5, DISABLE);

		DMA_ClearFlag(DMA1_FLAG_TC5);
		len = usart1_rx_buf_size - DMA_GetCurrDataCounter(DMA1_Channel5);
		if(len<0){
			len=0;
		}
        
		DMA_SetCurrDataCounter(DMA1_Channel5, usart1_rx_buf_size);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		
		(*USART1_OnRecvHandler)(len, usart1_rx_buffer);
		
		rec=USART_ReceiveData(USART1);
		rec=rec;
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	}
}

void DMA1_Channel4_IRQHandler() 
{
    if(DMA_GetITStatus(DMA1_IT_TC4) == SET){
        DMA_Cmd(DMA1_Channel4, DISABLE);
        (*USART1_OnSendHandler)();
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }
}


