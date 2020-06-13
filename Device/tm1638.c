#include "tm1638.h"

const u8 tm1638_light_levle = 6;

#define TM1638_READBYTES_CMD	    0X42
#define TM1638_DISPLAY_CMD			0X40
#define TM1638_ADDRESS_BASE	        0XC0
#define TM1638_LIGHT_CMD			0X88
#define TM1638_DISPLAY_OFF			0X80

#define TM1638_CS_PORT  MA
#define TM1638_CS_PIN   M4
#define TM1638_SPI_Init         SPI1M_Configuration
#define TM1638_ReadWriteByte    SPI1_ReadWriteByte
#define TM1638_ReadByte         SPI1_ReadByte
#define TM1638_Input            SPI1_Input
#define TM1638_Output           SPI1_Output

void TM1638_Enable(void);
void TM1638_Disable(void);
void TM1638_WriteCmd(u8 cmd);
void TM1638_WriteBytes(u8 cmd, u32 len, u8 *data);
void TM1638_ReadBytes(u8 addr, u32 len, u8 *buf);

void TM1638_Init(void)
{
    TM1638_SPI_Init();
    MG_Output(TM1638_CS_PORT, TM1638_CS_PIN);
    TM1638_Disable();
}

void TM1638_Enable(void)
{
    MG_Set(TM1638_CS_PORT, TM1638_CS_PIN, 0);
}

void TM1638_Disable(void)
{
    MG_Set(TM1638_CS_PORT, TM1638_CS_PIN, 1);
}

void TM1638_WriteCmd(u8 cmd)
{
    TM1638_Enable();
    TM1638_Output();
    TM1638_ReadWriteByte(cmd);
    TM1638_Disable();
}

void TM1638_WriteBytes(u8 cmd, u32 len, u8 *data)
{
    TM1638_Enable();
    TM1638_Output();
    TM1638_ReadWriteByte(cmd);
    while(len--){
        TM1638_ReadWriteByte(*data);
        data++;
    }
    TM1638_Disable();
}

void TM1638_ReadBytes(u8 addr, u32 len, u8 *buf)
{
    u8 i=0;
    
    TM1638_Enable();
    TM1638_Output();
    TM1638_ReadWriteByte(addr);
    
    TM1638_Input();
    for(i=0;i<len;i++){
        *buf=TM1638_ReadByte();
        buf++;
    }
    TM1638_Disable();
    TM1638_Output();
}

void TM1638_On(void)
{
    TM1638_WriteCmd(TM1638_LIGHT_CMD|6);
}

void TM1638_Off(void)
{
    TM1638_WriteCmd(TM1638_DISPLAY_OFF);
}

void TM1638_DisplayBit(u8 p, u8 n)
{
	TM1638_WriteCmd(TM1638_DISPLAY_CMD);
	TM1638_Enable();
	TM1638_ReadWriteByte(TM1638_ADDRESS_BASE|p);
	TM1638_ReadWriteByte(n);
	TM1638_Disable();
    TM1638_WriteCmd(TM1638_LIGHT_CMD|tm1638_light_levle);
}

void TM1638_Clear(void)
{
	u8 i;
	TM1638_WriteCmd(TM1638_DISPLAY_CMD);
	TM1638_Enable();
	TM1638_ReadWriteByte(TM1638_ADDRESS_BASE);
	for(i=0;i<16;i++){
		TM1638_ReadWriteByte(0X00);
	}
	TM1638_Disable();
	TM1638_WriteCmd(TM1638_LIGHT_CMD|tm1638_light_levle);
}

u32 TM1638_ReadKey(void)
{
    u8 buf[4];
    u32 data=0;
    u32 i=0;
    TM1638_ReadBytes(TM1638_READBYTES_CMD, 4, buf);
    for(i=0;i<4;i++){
        data=data<<8;
        data|=buf[i];
    }
    return data;
}
