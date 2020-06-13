#include "main.h"
#include <stdio.h>


u32 key=0;

// Simulate I2C
Mi2c mpu6050={
	.sda_gpio=MA,
	.scl_gpio=MA,
	.sda_pin=M4,
	.scl_pin=M4,
	.high_first=1,  // high bit first
	.time=5  // i2c speed
};

void get(s32 len, u8 *buf){
	// on receive data
}

void finish(){
	// on send data finish 
}

int main(void)
{	
    u8 i;
	int x=100;
	char buf[100];
    SystemInit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    delay_init();
		MG_RCC(MC);  // Use RCC: GPIOC
		MG_Output(MC,M13);  // Set GPIOC  PIN 13 Output
    MG_Set(MC,M13,0);  // Set Low
    
		MG_RCC(MA);  // Use RCC: GPIOC
		MG_Input(MA,M0, 1);  // Set GPIOA  PIN 0 Input Pull Up ( 0 for Pull Down, -1 for Float)
    i=MG_Get(MA,M0);  // Get Level
	
		USART1_Configuration();
		USART1_OnRecvHandler=get;  // set a callback function when recv a string
		USART1_OnSendHandler=finish; // set a callback function when dma send finish
	
		itoa(x, buf, 0);    // int to string
		x = atoi("-189");   // string to int
	
		MI2C_Init(&mpu6050);
		x=MI2C_ReadReg(&mpu6050,0x68, 0x01);
    
		while(1)
		{
			USART1_SendWords((u8*)"ABC",3);
			delay_ms(200);
			USART1_DMASendWords((u8*)"DEF",3);
			delay_ms(200);
		}
}

