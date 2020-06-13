#include "main.h"
#include <stdio.h>


u32 key=0;

int main(void)
{	
    u8 i;
    SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    delay_init();
    MG_RCC(MC);
	MG_Output(MC,M13);
    MG_Set(MC,M13,0);
    
    
    
    TM1638_Init();
    TM1638_On();
    TM1638_Clear();
    
	while(1)
	{
        TM1638_DisplayBit(2,0x0F);
		delay_ms(500);
        key=TM1638_ReadKey();
        TM1638_DisplayBit(2,0xF0);
		delay_ms(500);
        key=TM1638_ReadKey();
	}
}

