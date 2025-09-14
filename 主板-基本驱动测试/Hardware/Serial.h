#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint16_t text);
void Serial_SendArray(uint16_t* arr,uint16_t size);
void Serial_SendString(char* str);
uint32_t Serial_Pow(uint16_t x,uint16_t y);
void Serial_SendNumber(uint32_t num,uint16_t size);
void Serial_Printf(char* format,...);
uint16_t Serial_GetFlag(void);
uint16_t Serial_Getdata(void);
#endif
