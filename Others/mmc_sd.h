/*命令应答
R1:（置1有效），用于应答大部分命令的
7  6  5  4  3  2  1  0
0  |  |  |  |  |  |  |
   |  |  |  |  |  |  |______禁止状态（正在执行处理）
   |  |  |  |  |  |_________檫除复位
   |  |  |  |  |____________非法命令
   |  |  |  |_______________CRC错误
   |  |  |__________________檫除序列错误
   |  |_____________________地址错误
   |________________________参数错误

R1b：和R1有相同的格式，但添加了附加的忙信号。忙信号时连续8位为一组，全0时为忙，非全零时为准备好

写数据应答:
7  6  5  4  3  2  1  0
x  x  x  0  Status   1
Status="010":数据确认  "101":由于CRC错误被抛弃  "110":由于写入错误被抛弃

发送和接收的数据格式：（1）对于单块读、单块写和多块读：StartBlock（0xfe）+UserData（最大512字节）+CRC（2Bytes）
                            多块读通过发送CMD12来停止读取数据
                      （2）对于多块写：0xfc+UserData（最大512字节）+CRC（2Bytes)。发送StopTran（0xfd）来停止写入数据

读数据失败时的错误数据标识：（1Byte）7  6  5  4  3  2  1  0
                                     0  0  0  0  |  |  |  |___执行错误
                                                 |  |  |______卡内控制器错误
                                                 |  |_________卡内ECC校验错误
                                                 |____________地址超出卡的范围
*/

#ifndef _MMC_SD_DRIVER_H_
#define _MMC_SD_DRIVER_H_
 
#include "stm32f4xx.h"
#include "sys.h"
#include "spi.h"
#include "delay.h"
#include "string.h"

/* SD卡类型定义 */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SPI总线速度设置*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* SD传输数据结束后是否释放总线宏定义 */
#define NO_RELEASE      0
#define RELEASE         1
							  

/* SD卡指令表 */
#define CMD0    0      //卡复位  (应答格式：R1)
#define CMD1    1      //MMC卡开始初始化
#define CMD8		8     //识别卡的版本
#define CMD9    9     //命令9 ，读CSD数据   (应答格式：R1)
#define CMD10   10     //命令10，读CID数据   (应答格式：R1)
#define CMD12   12      //命令12，停止数据传输    (应答格式：R1b)
#define CMD16   16      //命令16，设置SectorSize 应返回0x00   (应答格式：R1)
#define CMD17   17      //命令17，读sector    (应答格式：R1)
#define CMD18   18      //命令18，读Multi sector    (应答格式：R1)
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block    (应答格式：R1)
#define CMD24   24      //命令24，写sector    (应答格式：R1)
#define CMD25   25     //命令25，写Multi sector    (应答格式：R1)
#define ACMD41  41      //命令41，应返回0x00    (应答格式：R1)
#define CMD55   55      //命令55，应返回0x01    (应答格式：R1)
#define CMD58   58      //命令58，读OCR信息     (应答格式：R1)
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00    (应答格式：R1)
					 
#define	SD_CS_ENABLE()  GPIO_ResetBits(GPIOB,GPIO_Pin_12) //选中SD卡
#define SD_CS_DISABLE() GPIO_SetBits(GPIOB,GPIO_Pin_12)	  //取消选中				    	 

typedef enum
{	 
	//特殊错误定义 
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),
	//标准错误定义
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,  
	SD_ERROR,  
	SD_OK = 0 
} SD_Error;	

//SD卡CSD寄存器数据		 
typedef struct
{
	u8  CSDStruct;            /*!< CSD structure */
	u8  SysSpecVersion;       /*!< System specification version */
	u8  Reserved1;            /*!< Reserved */
	u8  TAAC;                 /*!< Data read access-time 1 */
	u8  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	u8  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	u16 CardComdClasses;      /*!< Card command classes */
	u8  RdBlockLen;           /*!< Max. read data block length */
	u8  PartBlockRead;        /*!< Partial blocks for read allowed */
	u8  WrBlockMisalign;      /*!< Write block misalignment */
	u8  RdBlockMisalign;      /*!< Read block misalignment */
	u8  DSRImpl;              /*!< DSR implemented */
	u8  Reserved2;            /*!< Reserved */
	u32 DeviceSize;           /*!< Device Size */
	u8  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	u8  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	u8  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	u8  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	u8  DeviceSizeMul;        /*!< Device size multiplier */
	u8  EraseGrSize;          /*!< Erase group size */
	u8  EraseGrMul;           /*!< Erase group size multiplier */
	u8  WrProtectGrSize;      /*!< Write protect group size */
	u8  WrProtectGrEnable;    /*!< Write protect group enable */
	u8  ManDeflECC;           /*!< Manufacturer default ECC */
	u8  WrSpeedFact;          /*!< Write speed factor */
	u8  MaxWrBlockLen;        /*!< Max. write data block length */
	u8  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	u8  Reserved3;            /*!< Reserded */
	u8  ContentProtectAppli;  /*!< Content protection application */
	u8  FileFormatGrouop;     /*!< File format group */
	u8  CopyFlag;             /*!< Copy flag (OTP) */
	u8  PermWrProtect;        /*!< Permanent write protection */
	u8  TempWrProtect;        /*!< Temporary write protection */
	u8  FileFormat;           /*!< File Format */
	u8  ECC;                  /*!< ECC code */
	u8  CSD_CRC;              /*!< CSD CRC */
	u8  Reserved4;            /*!< always 1*/
} SD_CSD;  

//SD卡CID寄存器数据
typedef struct
{
	u8  ManufacturerID;       /*!< ManufacturerID */
	u16 OEM_AppliID;          /*!< OEM/Application ID */
	u32 ProdName1;            /*!< Product Name part1 */
	u8  ProdName2;            /*!< Product Name part2*/
	u8  ProdRev;              /*!< Product Revision */
	u32 ProdSN;               /*!< Product Serial Number */
	u8  Reserved1;            /*!< Reserved1 */
	u16 ManufactDate;         /*!< Manufacturing Date */
	u8  CID_CRC;              /*!< CID CRC */
	u8  Reserved2;            /*!< always 1 */
} SD_CID;	

//SD卡状态
typedef enum
{
	SD_CARD_READY                  = ((uint32_t)0x00000001),
	SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
	SD_CARD_STANDBY                = ((uint32_t)0x00000003),
	SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
	SD_CARD_SENDING                = ((uint32_t)0x00000005),
	SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
	SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
	SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
	SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;

typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  long long CardCapacity;  	//SD卡容量,单位:字节,最大支持2^64字节大小的卡.
  u32 CardBlockSize; 		//SD卡块大小	
  u16 RCA;					//卡相对地址
  u8 CardType;				//卡类型
} SD_CardInfo;
extern SD_CardInfo SDCardInfo;//SD卡信息	

/* Private function prototypes -----------------------------------------------*/
void SPI_ControlLine(void);
void SPI_SetSpeed_Local(u8 SpeedSet);

u8 SPI_ReadWriteByte(u8 TxData);                //SPI总线读写一个字节
u8 SD_WaitReady(void);                          //等待SD卡就绪
u8 SD_SendCommand(u8 cmd, u32 arg,u8 crc);     //SD卡发送一个命令
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg,u8 crc);
u8 SD_Init(void);                               //SD卡初始化
                                                //
u8 SD_ReceiveData(u8 *data, u16 len, u8 release);//SD卡读数据(OK)
u8 SD_GetCID(u8 *cid_data);                     //读SD卡CID(OK)
u8 SD_GetCSD(u8 *csd_data);                     //读SD卡CSD(OK)
u32 SD_GetCapacity(void);                       //取SD卡容量(OK)

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);  //读一个sector（OK)
u8 SD_WriteSingleBlock(u32 sector, const u8 *buffer); //写一个sector（OK)
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count); //读多个sector
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);  //写多个sector
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//读取一byte

u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);

#endif
