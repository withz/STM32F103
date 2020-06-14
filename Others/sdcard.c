#include "sdcard.h"

SD_Error SD_Status = SD_OK;
u32 sd_times=0;
char file_name_dir[30];

FRESULT SD_Mount(void);
FRESULT SDCard_CreateFile(FIL *fp);
FRESULT SDCard_AppendHead(FIL *fp);
FRESULT SDCard_CreateSettingFile(FIL *fp);
FRESULT SDCard_ReadSettingFile(FIL *fp);
FRESULT SDCard_WriteSettingFile(FIL *fp, u8 *set);

u8 SDCard_Configuration(void)	
{
	FRESULT res;
	u16 retry=0;
	u32 total,free;
	u8 exfun_res=0;
	
	while((SD_Status = SD_Init()) != SD_OK)
  {    
		sd_times++;	
		if(sd_times>40){
			SD_Status = SD_ERROR;
			return 1;
		}
  }
	
	exfun_res=exfuns_init();
	if(exfun_res!=0){
		SD_Status = SD_ERROR;
		return 1;
	}
	
	res=SD_Mount();
	if(res!=FR_OK){
		SD_Status = SD_ERROR;
		return res;
	}
	
	while(exf_getfree("0:",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		retry++;
		if(retry>200){
			SD_Status = SD_ERROR;
			return 1;
		}
	}
	
	if(SDCard_ReadSettingFile(file)!=FR_OK){
		SDCard_CreateSettingFile(file);
	}
	
	SDCard_CreateFile(file);
	return 0;
}

FRESULT SD_Mount(void)
{
	FRESULT res;
	res=f_mount(fs[0],"0:",1);
	if(res==FR_NO_FILESYSTEM){
		res=f_mkfs("0:",0,0);
		if(res==FR_OK){
			res=f_mount(NULL,"0:",1);
			res=f_mount(fs[0],"0:",1);
		}else{
			res=FR_NO_FILESYSTEM;
		}
	}
	if(res!=FR_OK){
		return res;
	}
	return FR_OK;
}



FRESULT SDCard_CreateFile(FIL *fp)
{
	u32 cnt=0;
	char cbuf[10];
	char name[30];
	
//	char* ext = ".csv\0";
	char* ext = ".rdt\0";
	FRESULT res=FR_NO_FILE;
	
	while(res != FR_OK){
		cnt++;
		itoa(cnt,cbuf,10);
		strcpy(name, "DATA");
		strcat(name, cbuf);
		strcat(name, ext);
		res = f_open(fp,name,FA_CREATE_NEW);
	}
	f_close(fp);
	strcpy(file_name_dir,name);
	return f_open(fp,name,FA_WRITE);
}

FRESULT SDCard_CloseFile(FIL *fp)
{
	return f_close(fp);
}

FRESULT SDCard_Write(FIL *fp, u8* data, u32 len)
{
	FRESULT res = FR_OK;
	res=f_write(fp, data, len, &bw);
	res=f_sync(file);
	return res;
}

FRESULT SDCard_AppendHead(FIL *fp)
{
	char* title="NUM0,NUM1,\
POUT00,POUT01,POUT02,POUT03,POUT04,POUT05,POUT06,POUT07,\
POUT08,POUT09,POUT10,POUT11,POUT12,POUT13,POUT14,\
AD00,AD01,AD02,AD03,AD04,AD05,AD06,AD07,AD08,AD09,AD10,AD11,AD12,AD13,AD14,AD15,AD16,\
PPE0,PPE1,PPE2,PPE3,PPE4,PPE5,PPE6,PPE7,\
PDT0,PDT1,PDT2,PDT3,PDT4,PDT5,PDT6,PDT7,\
\0";
	u16 title_len=0;
	FRESULT res = FR_OK;
	
	while(title[title_len++]!='\0'){
		if(title_len>1000){
			break;
		}
	}
	res=f_write(fp, title, title_len, &bw);
	f_sync(file);
	return res;
}

FRESULT SDCard_OpenDataFile(FIL *fp)
{
	FRESULT res=FR_NO_FILE;
	res = f_open(fp,file_name_dir,FA_OPEN_ALWAYS | FA_WRITE);
	res = f_lseek(fp, fp->fsize);
	return res;
}

FRESULT SDCard_CreateSettingFile(FIL *fp)
{
	FRESULT res=FR_NO_FILE;
	u8 *set = setting_buffer;
	res = f_open(fp,"setting.rst",FA_CREATE_NEW);
	f_close(fp);
	res = f_open(fp,"setting.rst",FA_WRITE);
	Setting_Init();
	res = f_write(fp,set,Setting_Capaticy(),&bw);
	res = f_close(fp);
	return res;
}

FRESULT SDCard_CheckSettingFile(FIL *fp)
{
	FRESULT res=FR_NO_FILE;
	res = f_open(fp,"setting.rst",FA_OPEN_EXISTING);
	return res;
}

FRESULT SDCard_ReadSettingFile(FIL *fp)
{
	FRESULT res=FR_NO_FILE;
	u8 *set = setting_buffer;
	res = f_close(fp);
	res = f_open(fp,"setting.rst",FA_READ);
	res = f_read(fp,set,Setting_Capaticy(),&bw);
	res = f_close(fp);
	return res;
}

FRESULT SDCard_WriteSettingFile(FIL *fp, u8* set)
{
	FRESULT res=FR_NO_FILE;
	res = f_close(fp);
	res = f_open(fp,"setting.rst",FA_WRITE);
	res = f_write(fp,set,Setting_Capaticy(),&bw);
	res = f_close(fp);
	return res;
}
