#include "stm32f10x.h"                  // Device header
#include <math.h>
#include "MyI2C.h" 
#include "OLED.h"
#include "MPU6050_RegAdd.h"

#define MPU6050_Address 0xD0
#define PI 3.14159
float AX,AY,AZ,GX,GY,GZ;
float Yaw,Pitch,Roll;


void MPU6050_WriteReg(uint8_t RegAddress,uint8_t data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address);
	MyI2C_RecieveACK();
	MyI2C_SendByte(RegAddress);
	MyI2C_RecieveACK();
	MyI2C_SendByte(data);
	MyI2C_RecieveACK();
	MyI2C_End();
	
}



uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t data;
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address);
	MyI2C_RecieveACK();
	MyI2C_SendByte(RegAddress);
	MyI2C_RecieveACK();
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address|0x01);
	MyI2C_RecieveACK();
	data=MyI2C_RecieveByte();
	MyI2C_SendACK(1);//不在接收
	MyI2C_End();
	return data;
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
	
}

void MPU6050_GetData(void)
{
	uint16_t DataH,DataL;
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	int16_t raw_AX=DataH<<8|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	int16_t raw_AY=DataH<<8|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	int16_t raw_AZ=DataH<<8|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	int16_t raw_GX=DataH<<8|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	int16_t raw_GY=DataH<<8|DataL;
	
	DataH=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	int16_t raw_GZ=DataH<<8|DataL;
	
//	AX=AX*6.1035e-5f;
//	AY=AY*6.1035e-5f;
//	AZ=AZ*6.1035e-5f;
//	GX=GX*6.1035e-2f;
//	GY=GY*6.1035e-2f;
//	GZ=GZ*6.1035e-2f;
	
	AX = raw_AX / 32768.0 * 16;
	AY = raw_AY / 32768.0 * 16;
	AZ = raw_AZ / 32768.0 * 16;
	GX = raw_GX / 32768.0 * 2000;
	GY = raw_GY / 32768.0 * 2000;
	GZ = raw_GZ / 32768.0 * 2000;
	
}

float MPU6050_ReturnAX(void)
{
	return AX;
}
float MPU6050_ReturnAY(void)
{
	return AY;
}
float MPU6050_ReturnAZ(void)
{
	return AZ;
}
float MPU6050_ReturnGX(void)
{
	return GX;
}
float MPU6050_ReturnGY(void)
{
	return GY;
}
float MPU6050_ReturnGZ(void)
{
	return GZ;
}
void MPU6050_GetAngle(void)
{
	
	if(fabs(GZ)>=1) Yaw=Yaw+GZ*0.1;
	

	Pitch=atan2(AX,AZ)*(180/PI);
	Roll=atan2(-AY,AZ)*(180/PI);
	

}

float MPU6050_ReturnYaw(void)
{
	return Yaw;
}
float MPU6050_ReturnPitch(void)
{
	return Pitch;
}
float MPU6050_ReturnRoll(void)
{
	return Roll;
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
