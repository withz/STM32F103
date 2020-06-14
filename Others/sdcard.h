#ifndef _SDCARD_H_
#define _SDCARD_H_

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

#include "bsp_sdio_sd.h"
#include "malloc.h"
#include "string.h"

#include "diskio.h"
#include "ff.h"
#include "exfuns.h"

#include "settingfile.h"

u8  SD_write_command(u8 command, u32 arg, u8 crc);

#define SD_CMD0		0X0001
#define SD_CMD8		0X01AA
#define SD_CMD55	0X01
#define SD_ACMD41	0X1

#define FILE_RAW_DATA_LEN		1024

/* This type MUST be 8 bit */
typedef unsigned char	BYTE;

/* These types MUST be 16 bit */
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types MUST be 16 bit or 32 bit */
typedef int				INT;
typedef unsigned int	UINT;

/* These types MUST be 32 bit */
typedef long			LONG;
typedef unsigned long	DWORD;

extern u8 file_buf[FILE_RAW_DATA_LEN];
extern u16 file_buf_ptr;
extern SD_Error SD_Status;

u8 SDCard_Configuration(void);
FRESULT SDCard_Write(FIL *fp, u8* data, u32 len);
FRESULT SDCard_WriteSettingFile(FIL *fp, u8 *set);
FRESULT SDCard_ReadSettingFile(FIL *fp);
FRESULT SDCard_CreateSettingFile(FIL *fp);
FRESULT SDCard_CheckSettingFile(FIL *fp);
FRESULT SDCard_OpenDataFile(FIL *fp);

#endif


