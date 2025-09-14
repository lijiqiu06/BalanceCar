//主板用
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "Key.h"
#include "Timer.h"
#include "Motor.h"
#include "encoder.h"
#include "MPU6050.h"
#include "Serial.h"



//#define KEY_AND_2_4G_TEST
//#define MOTOR_AND_ENCODER_TEST
//#define MPU6050_TEST
//#define SERIAL_TEST

#ifdef KEY_AND_2_4G_TEST
uint8_t SendFlag;								//发送标志位
uint8_t SendSuccessCount, SendFailedCount;		//发送成功计次，发送失败计次

uint8_t ReceiveFlag;							//接收标志位
uint8_t ReceiveSuccessCount, ReceiveFailedCount;//接收成功计次，接收失败计次
#endif

#ifdef MOTOR_AND_ENCODER_TEST

int SpeedL,SpeedR;
#endif

#ifdef MPU6050_TEST

float M_Pitch;
#endif
int main(void)
{
	/*初始化*/
	OLED_Init();
	Key_Init();
	NRF24L01_Init();
	Timer_Init();
	MotorL_Init();
	MotorR_Init();
	EncoderL_Init();
	EncoderR_Init();
	MPU6050_Init();
	Serial_Init();
#ifdef KEY_AND_2_4G_TEST
	/*OLED显示静态字符串*/
	OLED_ShowString(0, 0, "T:000-000-0",OLED_8X16);		//格式为：T:发送成功计次-发送失败计次-发送标志位
	OLED_ShowString(0, 32, "R:000-000-0",OLED_8X16);		//格式为：R:接收成功计次-接收失败计次-接收标志位
	OLED_Update();
	/*初始化测试数据，此处值为任意设定，便于观察实验现象*/
	NRF24L01_TxPacket[0] = 0x00;
	NRF24L01_TxPacket[1] = 0x01;
	NRF24L01_TxPacket[2] = 0x02;
	NRF24L01_TxPacket[3] = 0x03;
	
//	NRF24L01_W_CSN(1);
//	NRF24L01_W_SCK(1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_4,(BitAction)1);
	while (1)
	{
//		OLED_ShowNum(1,1,NRF24L01_R_MISO(),1);
		
		
		if (Key_Check(1,KEY_CLICK))				//K1按下
		{
			/*变换测试数据，便于观察实验现象*/
			/*实际项目中，可以将待发送的数据赋值给NRF24L01_TxPacket数组*/
			NRF24L01_TxPacket[0] ++;
			NRF24L01_TxPacket[1] ++;
			NRF24L01_TxPacket[2] ++;
			NRF24L01_TxPacket[3] ++;
			
			/*调用NRF24L01_Send函数，发送数据，同时返回发送标志位，方便用户了解发送状态*/
			/*发送标志位与发送状态的对应关系，可以转到此函数定义上方查看*/
			SendFlag = NRF24L01_Send();
			
			if (SendFlag == 1)			//发送标志位为1，表示发送成功
			{
				SendSuccessCount ++;	//发送成功计次变量自增
			}
			else						//发送标志位不为1，即2/3/4，表示发送不成功
			{
				SendFailedCount ++;		//发送失败计次变量自增
			}
			
			OLED_ShowNum(16, 0, SendSuccessCount, 3,OLED_8X16);	//显示发送成功次数
			OLED_ShowNum(48, 0, SendFailedCount, 3,OLED_8X16);		//显示发送失败次数
			OLED_ShowNum(80, 0, SendFlag, 1,OLED_8X16);			//显示最近一次的发送标志位
			
			/*显示发送数据*/
			OLED_ShowHexNum(0, 16, NRF24L01_TxPacket[0], 2,OLED_8X16);
			OLED_ShowHexNum(24, 16, NRF24L01_TxPacket[1], 2,OLED_8X16);
			OLED_ShowHexNum(48, 16, NRF24L01_TxPacket[2], 2,OLED_8X16);
			OLED_ShowHexNum(72, 16, NRF24L01_TxPacket[3], 2,OLED_8X16);
			
			OLED_Update();
		}
		
		/*主循环内循环执行NRF24L01_Receive函数，接收数据，同时返回接收标志位，方便用户了解接收状态*/
		/*接收标志位与接收状态的对应关系，可以转到此函数定义上方查看*/
		ReceiveFlag = NRF24L01_Receive();
		
		if (ReceiveFlag)				//接收标志位不为0，表示收到了一个数据包
		{
			if (ReceiveFlag == 1)		//接收标志位为1，表示接收成功
			{
				ReceiveSuccessCount ++;	//接收成功计次变量自增
			}
			else	//接收标志位不为0也不为1，即2/3，表示此次接收产生了错误，错误接收的数据不应该使用
			{
				ReceiveFailedCount ++;	//接收失败计次变量自增
			}
			
			OLED_ShowNum(16, 32, ReceiveSuccessCount, 3,OLED_8X16);	//显示接收成功次数
			OLED_ShowNum(48, 32, ReceiveFailedCount, 3,OLED_8X16);	//显示接收失败次数
			OLED_ShowNum(80, 32, ReceiveFlag, 1,OLED_8X16);		//显示最近一次的接收标志位
			
			/*显示接收数据*/
			OLED_ShowHexNum(0, 48, NRF24L01_RxPacket[0], 2,OLED_8X16);
			OLED_ShowHexNum(24, 48, NRF24L01_RxPacket[1], 2,OLED_8X16);
			OLED_ShowHexNum(48, 48, NRF24L01_RxPacket[2], 2,OLED_8X16);
			OLED_ShowHexNum(72, 48, NRF24L01_RxPacket[3], 2,OLED_8X16);
			
			
			
			OLED_Update();
		}
	}
	
#endif
	
#ifdef MOTOR_AND_ENCODER_TEST
	int PWML=0,PWMR=0;
	
	while(1)
	{
		if(Key_Check(1,KEY_CLICK))
		{
			PWML+=10;
			if(PWML>=100) PWML=100;
			else if(PWML<=-100) PWML=-100;
		}
		
		if(Key_Check(2,KEY_CLICK))
		{
			PWML-=10;
			if(PWML>=100) PWML=100;
			else if(PWML<=-100) PWML=-100;
		}
		
		if(Key_Check(3,KEY_CLICK))
		{
			PWMR+=10;
			if(PWMR>=100) PWMR=100;
			else if(PWMR<=-100) PWMR=-100;
		}
		
		if(Key_Check(4,KEY_CLICK))
		{
			PWMR-=10;
			if(PWMR>=100) PWMR=100;
			else if(PWMR<=-100) PWMR=-100;
		}
		
		MotorL_SetPWM(PWML);
		MotorR_SetPWM(PWMR);
		
		OLED_Printf(0,0,OLED_8X16,"PWML=%3d",PWML);
		OLED_Printf(0,16,OLED_8X16,"PWMR=%3d",PWMR);
		OLED_Printf(0,32,OLED_8X16,"SpeedL=%3d",SpeedL);
		OLED_Printf(0,48,OLED_8X16,"SpeedR=%3d",SpeedR);
		OLED_Update();
	}
#endif
	
#ifdef MPU6050_TEST
	OLED_ShowHexNum(0,0,MPU6050_GetID(),2,OLED_8X16);
	while(1)
	{
		OLED_Printf(0,16,OLED_8X16,"Pitch=%.2f",M_Pitch);
		OLED_Update();
	}

#endif
	
#ifdef SERIAL_TEST
	uint8_t data[3]={0x00,0x01,0x02};
	while(1)
	{
		Serial_Printf("%X %X %X\r\n",data[0],data[1],data[2]);
		data[0]++;
		data[1]++;
		data[2]++;
	}
#endif
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		static uint16_t count=0,count2=0;
		count++;
		count2++;
		if(count>=40)
		{
			count=0;
			#ifdef MOTOR_AND_ENCODER_TEST
			SpeedR=EncoderR_GetCount();
			SpeedL=EncoderL_GetCount();
			#endif
			
			#ifdef MPU6050_TEST
			M_Pitch=MPU6050_ReturnPitch();
			//OLED_Printf(0,48,OLED_8X16,"1");
			#endif
		}
		
		if(count2>=10)
		{
			count2=0;
			#ifdef MPU6050_TEST
			MPU6050_GetData();
			MPU6050_GetAngle();
			
			#endif
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}