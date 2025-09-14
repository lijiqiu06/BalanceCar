#include "stm32f10x.h"                  // Device header
#include "PID.h"

void PID_Update(PID_t* PIDStruct)
{
	PIDStruct->Errorpre=PIDStruct->Errorcur;
	PIDStruct->Errorcur=PIDStruct->Target-PIDStruct->Actual;
	
	if (PIDStruct->Ki != 0)				
	{
		PIDStruct->ErrorInt += PIDStruct->Errorcur;		
	}
	else						
	{
		PIDStruct->ErrorInt = 0;			
	}
	PIDStruct->Out=PIDStruct->Kp*PIDStruct->Errorcur + PIDStruct->Ki*PIDStruct->ErrorInt + PIDStruct->Kd*(PIDStruct->Errorcur-PIDStruct->Errorpre);
	if(PIDStruct->Out>PIDStruct->max)PIDStruct->Out=PIDStruct->max;
	if(PIDStruct->Out<PIDStruct->min)PIDStruct->Out=PIDStruct->min;
}
