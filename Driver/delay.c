#include "delay.h"

static unsigned int fac_us=0;
static unsigned int fac_ms=0;

void delayms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++){
		int a=10300;
 		while(a--);
	}
}

void delayus(unsigned int t)
{
	int i;
	for( i=0;i<t;i++){
		int a=9;
		while(a--);
	}
}

void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SystemCoreClock/8000000;
	fac_ms=fac_us*1000;
}

void delay_us(unsigned int nus)
{		
	u32 temp;
	SysTick->LOAD=nus*fac_us;
	SysTick->VAL=0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

void delay_ms(unsigned int nms)
{
	u32 temp;
    u8 s;
    
    if(nms>1000){
        s=nms/1000;
        while(s--){
            delay_ms(1000);
        }
    }
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
	do{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
} 

