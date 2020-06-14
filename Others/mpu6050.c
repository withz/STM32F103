#include "mpu6050.h"

MPU_Data mpu_data;
MPU_Pose mpu_pose;
MPU_State mpu_state;
float gravityValue=0;

u8 acc_fsr=2;
u16 gyr_fsr=2000;

Mi2c mpu6050={
	.highFirst=1,
	.scl_gpio=MA,
	.sda_gpio=MA,
	.sda_pin=M3,
	.scl_pin=M4,
	.time=2
};

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
float Pitch,Roll,Yaw;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
u8 mpu_address;

u8 MPU6050_Configuration(u16 rate)
{
	u8 mpu9050=0;
	u8 re=0;
	MI2C_Init(&mpu6050);
	MPU6050_Cmd(MPU_PWR_MGMT1_REG,0X80);				//复位MPU
	delay_ms(10);
	MPU6050_Cmd(MPU_PWR_MGMT1_REG,0X00);				//唤醒MPU
	MPU6050_Cmd(MPU_GYRO_CFG_REG,3<<3);					//陀螺仪  2000dps
	MPU6050_Cmd(MPU_ACCEL_CFG_REG,0<<3);				//加速计  2g
	MPU6050_Cmd(MPU_SAMPLE_RATE_REG,MPU_Set_Rate(rate));	//采样率
	MPU6050_Cmd(MPU_CFG_REG,MPU_Set_LPF(rate/2));
	MPU6050_Cmd(MPU_INT_EN_REG,0X00);					//关闭中断
	MPU6050_Cmd(MPU_USER_CTRL_REG,0X00);				//关闭I2C主模式
	MPU6050_Cmd(MPU_FIFO_EN_REG,0X00);					//关闭FIFO
	MPU6050_Cmd(MPU_INTBP_CFG_REG,0X80);				//INT低电平有效
	
	re=MPU6050_Read(MPU_DEVICE_ID_REG);					//读取器件地址
	
	MPU6050_Cmd(MPU_PWR_MGMT1_REG,0X01);				//设置X轴为参考
	MPU6050_Cmd(MPU_PWR_MGMT2_REG,0X00);				//加速计，陀螺仪工作
	MPU6050_Cmd(MPU_SAMPLE_RATE_REG,MPU_Set_Rate(rate));	//设置采样率
	MPU6050_Cmd(MPU_CFG_REG,MPU_Set_LPF(rate/2));
	
	MPU6050_Cmd(MPU_INTBP_CFG_REG,0X02);				//INT低电平有效
	
	mpu_init();
	
	mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	mpu_set_sample_rate(DEFAULT_MPU_HZ);
	dmp_load_motion_driver_firmware();
	dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
	dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
			DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
			DMP_FEATURE_GYRO_CAL);
				
	dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	run_self_test();
	mpu_set_dmp_state(1);

	MPU6050_MarkState();
	
	mpu_address=re;
	return re;
}

u8 MPU6050_GetData(MPU_Data* m)
{
	u8 re=0;
	u8 buf[6];
	float t;
	s16 tt;
	re+=MPU6050_Read_Len(MPU_ACCEL_XOUTH_REG,6,buf);
	if(re>0){
		return 1;
	}
	m->ax=(s16)((buf[0]<<8)|(buf[1]));
	m->ay=(s16)((buf[2]<<8)|(buf[3]));
	m->az=(s16)((buf[4]<<8)|(buf[5]));
	
	re+=MPU6050_Read_Len(MPU_GYRO_XOUTH_REG,6,buf);
	if(re>0){
		return 1;
	}
	m->gx=(s16)((buf[0]<<8)|(buf[1]));
	m->gy=(s16)((buf[2]<<8)|(buf[3]));
	m->gz=(s16)((buf[4]<<8)|(buf[5]));
	
	re+=MPU6050_Read_Len(MPU_TEMP_OUTH_REG,2,buf);
	if(re>0){
		return 1;
	}
	tt=((s16)buf[0]<<8)|buf[1];
	t=36.53+((double)tt)/340;
	t*=100;
	m->Temp=(s16)t;
	return re;
}

void MPU6050_GetPose(MPU_Pose* t)
{
	MPU6050_Pose();
	t->Pitch=Pitch;
	t->Roll=Roll;
	t->Yaw=Yaw;
}

void MPU6050_GetState(MPU_State* m)
{
	MPU6050_GetData(&mpu_data);
	m->ax=mpu_data.ax*2*9.8/32768.0;
	m->ay=mpu_data.ay*2*9.8/32768.0;
	m->az=mpu_data.az*2*9.8/32768.0;
	
	m->gx=mpu_data.gx*2000/32768.0;
	m->gy=mpu_data.gy*2000/32768.0;
	m->gz=mpu_data.gz*2000/32768.0;
}

void MPU6050_GetRelativeState(MPU_State* m)
{
	float gx,gy,gz;
	MPU6050_GetState(&mpu_state);
	MPU6050_GetPose(&mpu_pose);
	gx=gravityValue*sinf(mpu_pose.Pitch/57.3);
	gz=gravityValue*cosf(mpu_pose.Pitch/57.3);
	gy=gz*sinf(mpu_pose.Roll/57.3);
	gz=gz*cosf(mpu_pose.Roll/57.3);
	
	m->ax=m->ax-gx;
	m->ay=m->ay+gy;
	m->az=m->az-gz;
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	switch(fsr){
		case 0:gyr_fsr=250;break;
		case 1:gyr_fsr=500;break;
		case 2:gyr_fsr=1000;break;
		case 3:gyr_fsr=2000;break;
	}
	return MPU6050_Cmd(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	switch(fsr){
		case 0:acc_fsr=2;break;
		case 1:acc_fsr=4;break;
		case 2:acc_fsr=8;break;
		case 3:acc_fsr=16;break;
	}
	return MPU6050_Cmd(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU6050_Cmd(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 rate) 
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU6050_Cmd(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

void MPU6050_Pose(void)
{
	float q30=1024*1024*1024;
	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	 

	if(sensors & INV_WXYZ_QUAT )
	{
		q0 = quat[0] / q30;	
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;

		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}
}

void MPU6050_MarkState(void)
{
	u8 i;
	u8 times=100;
	gravityValue=0;
	for(i=0;i<times;i++)
	{
		MPU6050_GetState(&mpu_state);
		gravityValue+=sqrtf(mpu_state.ax*mpu_state.ax+mpu_state.ay*mpu_state.ay+mpu_state.az*mpu_state.az)/times;
		delay_ms(10);
	}
}

u8 MPU6050_Cmd(u8 reg,u8 cmd)
{
	u8 re=0;
	MI2C_Start(&mpu6050);
	
	MI2C_Write(&mpu6050,(MPU_ADDR<<1)|0);
	re+=MI2C_GetAck(&mpu6050);
	
	MI2C_Write(&mpu6050,reg);
	re+=MI2C_GetAck(&mpu6050);
	
	MI2C_Write(&mpu6050,cmd);
	re+=MI2C_GetAck(&mpu6050);
	
	MI2C_End(&mpu6050);
	return re;
}

u8 MPU6050_Read(u8 reg)
{
	u8 data;
	MI2C_Start(&mpu6050);
	
	MI2C_Write(&mpu6050,(MPU_ADDR<<1)|0);
	MI2C_GetAck(&mpu6050);
	
	MI2C_Write(&mpu6050,reg);
	MI2C_GetAck(&mpu6050);
	
	MI2C_Start(&mpu6050);
	
	MI2C_Write(&mpu6050,(MPU_ADDR<<1)|1);
	MI2C_GetAck(&mpu6050);
	data=MI2C_Read(&mpu6050);
	MI2C_SetNoAck(&mpu6050);
	
	MI2C_End(&mpu6050);
	
	return data;
}

u8 MPU6050_Read_Len(u8 reg,u8 len,u8 *buf)
{
	u8 re=0;
	MI2C_Start(&mpu6050); 
	MI2C_Write(&mpu6050,(MPU_ADDR<<1));		//发送器件地址+写命令	
	re+=MI2C_GetAck(&mpu6050);
	
    MI2C_Write(&mpu6050,reg);						//写寄存器地址
    re+=MI2C_GetAck(&mpu6050);							//等待应答
    MI2C_Start(&mpu6050);
	MI2C_Write(&mpu6050,(MPU_ADDR<<1)|1);		//发送器件地址+读命令	
    re+=MI2C_GetAck(&mpu6050);							//等待应答 
	while(len)
	{
		*buf=MI2C_Read(&mpu6050);
		if(len==1){
			MI2C_SetNoAck(&mpu6050);				//发送nACK 
		}
		else{
			MI2C_SetAck(&mpu6050);					//发送ACK  
		}
		len--;
		buf++; 
	}    
    MI2C_End(&mpu6050);								//产生一个停止条件 	
	return re;
}

u8 MPU_Write_Len(u8 reg,u8 len,u8 *buf)
{
	u8 re=0;
	u8 i; 
    MI2C_Start(&mpu6050); 
	MI2C_Write(&mpu6050,(MPU_ADDR<<1)&0XFE);		//发送器件地址+写命令	
	re+=MI2C_GetAck(&mpu6050);
	
    MI2C_Write(&mpu6050,reg);						//写寄存器地址
    re+=MI2C_GetAck(&mpu6050);
	for(i=0;i<len;i++)
	{
		MI2C_Write(&mpu6050,buf[i]);				//发送数据
		re+=MI2C_GetAck(&mpu6050);						//等待ACK
			
	}    
    MI2C_End(&mpu6050);	 
	return re;
} 

//-----INV
int MPU6050_ReadDMP(u8 addr, u8 reg, u8 len, u8 *buf)
{
	if(addr==MPU_ADDR){
		return MPU6050_Read_Len(reg,len,buf);
	}else if(addr==COMPASS_ADDR){
		return Compass_Read_Len(reg,len,buf);
	}
	return 0;
}

int MPU6050_WriteDMP(u8 addr, u8 reg, u8 len, u8 *buf)
{
	if(addr==MPU_ADDR){
		return MPU_Write_Len(reg,len,buf);
	}else if(addr==COMPASS_ADDR){
		return Compass_Write_Len(reg,len,buf);
	}
	return 0;
}




