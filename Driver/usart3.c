#include "usart3.h"

const u32 usart3_rx_buf_size = 256;
const u32 usart3_tx_buf_size = 256;

u8 usart3_rx_buffer[usart3_rx_buf_size];
u8 usart3_tx_buffer[usart3_tx_buf_size];

void USART3_OnRecv(s32 len, u8* c);
void USART3_OnSend(void);

void (*USART3_OnRecvHandler)(s32 len, u8* c) = USART3_OnRecv;
void (*USART3_OnSendHandler)() = USART3_OnSend;

void USART3_Configuration(void)		//´®¿Ú³õÊ¼»¯--USART3--PB10--TX--PB11--RX--
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_10;  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio);

    USART_DeInit(USART3);
    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART3,&usart);
	
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)usart3_rx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = usart3_rx_buf_size;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3,&dma);
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)usart3_tx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralDST;
    dma.DMA_BufferSize = usart3_tx_buf_size;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2,&dma);
 
    nvic.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = DMA1_Channel2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
    
    DMA_Cmd(DMA1_Channel3, ENABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
    
    USART_Cmd(USART3,ENABLE);
}

void USART3_SendChar(u8 b)		//·¢ËÍ×Ö·û
{
    unsigned short wait = 60000;
    while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET){
        if(wait < 10) return;
        wait--;
	}
	USART_SendData(USART3,b);
}

void USART3_SendWords(u8* c, s32 len)		//·¢ËÍ×Ö·û´®
{
	unsigned short wait = 60000;
	while(len--){
        wait = 60000;
		while( USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET){
            if(wait < 10) return;
            wait--;
        }
		USART_SendData(USART3,*c);
		c++;
	}
}


void USART3_DMASendWords(u8 *c, int len)
{
	u32 i;
    u32 n=0;
    u32 part=len/usart3_tx_buf_size;
    
    for(n=0;n<part;n++){
        while(DMA_GetFlagStatus(DMA1_FLAG_TC2)!=RESET);
        for(i=0;i<usart3_tx_buf_size;i++){
            usart3_tx_buffer[i]=c[i+n*usart3_tx_buf_size];
        }
        DMA_SetCurrDataCounter(DMA1_Channel2, usart3_tx_buf_size);
        DMA_Cmd(DMA1_Channel2, ENABLE);
        len=len-usart3_tx_buf_size;
    }
    while(DMA_GetFlagStatus(DMA1_FLAG_TC2)!=RESET);
    for(i=0;i<len;i++){
        usart3_tx_buffer[i]=c[i+n*usart3_tx_buf_size];
    }
    DMA_SetCurrDataCounter(DMA1_Channel2, len);
    DMA_Cmd(DMA1_Channel2, ENABLE);
}

void USART3_OnRecv(s32 len, u8* c)
{
    
}

void USART3_OnSend()
{
    
}


void USART3_IRQHandler()
{
    u8 rec=0;
    s32 len=0;

    if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET){
        DMA_Cmd(DMA1_Channel3, DISABLE);

		DMA_ClearFlag(DMA1_FLAG_TC3);
		len = usart3_rx_buf_size - DMA_GetCurrDataCounter(DMA1_Channel3);
		if(len<0){
			len=0;
		}
        
		DMA_SetCurrDataCounter(DMA1_Channel3, usart3_rx_buf_size);
		DMA_Cmd(DMA1_Channel3, ENABLE);
		
		(*USART3_OnRecvHandler)(len, usart3_rx_buffer);
		
		rec=USART_ReceiveData(USART3);
		rec=rec;
		USART_ClearITPendingBit(USART3,USART_IT_IDLE);
	}
}

void DMA1_Channel2_IRQHandler() 
{
    if(DMA_GetITStatus(DMA1_IT_TC2) == SET){
        DMA_Cmd(DMA1_Channel2, DISABLE);
        (*USART3_OnSendHandler)();
        DMA_ClearFlag(DMA1_FLAG_TC2);
        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }
}

