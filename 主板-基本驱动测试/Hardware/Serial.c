#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint16_t data;
uint16_t flag;
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint16_t text)
{
	USART_SendData(USART1,text);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
}

void Serial_SendArray(uint16_t* arr,uint16_t size)
{
	for(uint16_t i=0;i<size;i++)
	{
		Serial_SendByte(arr[i]);
	}
}

void Serial_SendString(char* str)
{
	for(uint16_t i=0;str[i]!=0;i++)
	{
		Serial_SendByte(str[i]);
	}
}

uint32_t Serial_Pow(uint16_t x,uint16_t y)
{
	uint32_t ret=1;
	for(uint16_t i=0;i<y;i++)
	{
		ret*=x;
	}
	return ret;
}

void Serial_SendNumber(uint32_t num,uint16_t size)
{
	for(uint16_t i=0;i<size;i++)
	{
		uint32_t temp;
		temp=(num/Serial_Pow(10,size-i-1))%10;
		Serial_SendByte(temp+'0');
	}
}

int fputc(int ch,FILE* f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char* format,...)
{
	char str[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(str,format,arg);
	va_end(arg);
	Serial_SendString(str);	
}

uint16_t Serial_GetFlag(void)
{
	if(flag==1)
	{
		flag=0;
		return 1;
	}
	return 0;
}

uint16_t Serial_Getdata(void)
{
	return data;
}

void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		data=USART_ReceiveData(USART1);
		flag=1;
	}
}

