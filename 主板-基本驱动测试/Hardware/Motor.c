#include "stm32f10x.h"                  // Device header
#include "PWM.h"
//L->A1
void MotorL_Init(void)
{
	PWM_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	

}

void MotorR_Init(void)
{
	PWM_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	

}

void MotorL_SetPWM(int16_t PWM)
{
	if(PWM>=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		PWM_SetCompare2(PWM);
	}
		
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	  GPIO_SetBits(GPIOB,GPIO_Pin_15);
		PWM_SetCompare2(-PWM);
	}
	
}

void MotorR_SetPWM(int16_t PWM)
{
	if(PWM>=0)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	  GPIO_SetBits(GPIOB,GPIO_Pin_13);
		PWM_SetCompare1(PWM);
	}
		
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		PWM_SetCompare1(-PWM);
	}
	
}


