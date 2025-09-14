#ifndef _MPU6050_H
#define _MPU6050_H
void MPU6050_Init(void);
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_GetData(void);
void MPU6050_GetAngle(void);
uint8_t MPU6050_GetID(void);
float MPU6050_ReturnAX(void);
float MPU6050_ReturnAY(void);
float MPU6050_ReturnAZ(void);
float MPU6050_ReturnGX(void);
float MPU6050_ReturnGY(void);
float MPU6050_ReturnGZ(void);
float MPU6050_ReturnYaw(void);
float MPU6050_ReturnPitch(void);
float MPU6050_ReturnRoll(void);
#endif 
