#ifndef _MPU6050_H_
#define _MPU6050_H_

//精简版
#define SMPLRT_DIV		0x19			//采样分辨率125Hz
#define MPU_CONFIG		0x1A      		//低通滤波5Hz
#define GYRO_CONFIG  	0x1B  			//陀螺仪自检和测量范围 不自检 2000deg/s
#define ACCEL_CONFIG 	0x1C  			//加速机自检，测量范围，高通滤波 不自检 2G 5Hz
 
#define ACCEL_XOUT_H 	0x3B  			//最近一次加速计值
#define ACCEL_XOUT_L 	0x3C
#define ACCEL_YOUT_H 	0x3D
#define ACCEL_YOUT_L 	0x3E
#define ACCEL_ZOUT_H 	0x3F
#define ACCEL_ZOUT_L 	0x40
 
#define TEMP_OUT_H  	0x41   			//最近一次温度值
#define TEMP_OUT_L  	0x42
 
#define GYRO_XOUT_H  	0x43 			//最近一次陀螺仪值
#define GYRO_XOUT_L  	0x44 
#define GYRO_YOUT_H  	0x45
#define GYRO_YOUT_L  	0x46
#define GYRO_ZOUT_H  	0x47
#define GYRO_ZOUT_L  	0x48
 
#define PWR_MGMT_1  	0x6B 			//电源管理  正常使用
#define WHO_AM_I  		0x75 			//I2C地址

//扩展版
#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
 
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU_ADDR				0X68

#define ACC_G						9.80

#include "stm32f10x.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"

typedef struct MPU6050_Data{
	s16 Temp;
	s16 gx;
	s16 gy;
	s16 gz;
	s16 ax;
	s16 ay;
	s16 az;
} MPU_Data;

typedef struct MPU6050_Pose{
	float Roll;
	float Pitch;
	float Yaw;
} MPU_Pose;

typedef struct MPU6050_State{
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
} MPU_State;

extern MPU_Data mpu_data;
extern MPU_Pose mpu_pose;
extern MPU_State mpu_state;

u8 MPU6050_Configuration(u16 rate);

u8 MPU6050_GetData(MPU_Data* m);
void MPU6050_GetPose(MPU_Pose* t);
void MPU6050_Pose(void);
void MPU6050_GetState(MPU_State* m);
void MPU6050_GetRelativeState(MPU_State* m);
void MPU6050_MarkState(void);

u8 MPU6050_Read(u8 reg);
u8 MPU6050_Read_Len(u8 reg,u8 len,u8 *buf);
u8 MPU6050_Cmd(u8 reg,u8 cmd);
u8 MPU_Write_Len(u8 reg,u8 len,u8 *buf);

u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);

int MPU6050_WriteDMP(u8 addr, u8 reg, u8 len, u8 *buf);
int MPU6050_ReadDMP(u8 addr, u8 reg, u8 len, u8 *buf);













#endif

