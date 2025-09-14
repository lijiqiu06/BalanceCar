#ifndef __PID_H
#define __PID_H
typedef struct 
{
	float Target,Actual,Out;
	float Kp,Ki,Kd;
	float Errorpre,Errorcur,ErrorInt;
	int32_t max,min;
}PID_t;
void PID_Update(PID_t* PIDStruct);
#endif
