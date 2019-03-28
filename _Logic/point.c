#include "point.h"
static int _times=5;
static int _LastTimeButtomValue=0;
uint32_t _DValue[7];
int PointFlag=0;
uint32_t _FindMax(uint32_t L[])
{
	uint32_t _max=0;
	for(int i=0;i<=6;i++)
	{
		if(L[i]>=_max)
		{
			_max=L[i];
		}
	}
	return _max;
}
uint32_t _CalculationADC(void)
{
		const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t temp=0;
	for(int i=0; i<=7; i++)
	{
		temp+=adcData->array[0][i];
	}
	return temp;
}
uint32_t _UpdateButtomValue(void)
{
	if(_times==5)
	{
		_times=0;
		return _CalculationADC();
	}
	else
	{
		_times++;
		return _LastTimeButtomValue;
	}
	
}
void _FindPointStop(void)
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t MaxDvalue;
	uint32_t _Time=0;
	_LastTimeButtomValue=_UpdateButtomValue();
	for(int i=0; i<=6; i++)
	{
		_DValue[i]=abs(adcData->array[0][i]-adcData->array[0][i+1]);
		//printf("%d,",_DValue[i]);
	}
	//printf("\r\n");
	MaxDvalue=_FindMax(_DValue);
	//printf("%d\r\n",MaxDvalue);
	
	for(int i=0;i<=6;i++)
	{
		if(MaxDvalue/2>_DValue[i])
		{
			_Time++;
		}
	}
	//printf("%d",_Time);
			if(_Time<=4)
		{
			PointFlag++;
			if(PointFlag>=2)
			{
			//printf("1");
			FlagPoint=1;
			UpdateMotorState(MOTOR_STOP);
			ClearLLaserChangePendingBit();
			ClearRLaserChangePendingBit();
			PointFlag=0;
			}
			
		}
		else
		{
			ClearLLaserChangePendingBit();
			ClearRLaserChangePendingBit();
		}
}
void _FindPointGo()
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t MaxDvalue;
	uint32_t _Time=0;
	_LastTimeButtomValue=_UpdateButtomValue();
	for(int i=0; i<=6; i++)
	{
		_DValue[i]=abs(adcData->array[0][i]-adcData->array[0][i+1]);
		//printf("%d,",_DValue[i]);
	}
	//printf("\r\n");
	MaxDvalue=_FindMax(_DValue);
	//printf("%d\r\n",MaxDvalue);
	
	for(int i=0;i<=6;i++)
	{
		if(MaxDvalue/2>_DValue[i])
		{
			_Time++;
		}
	}
	if(_Time<=4||adcData->array[0][0]<3200||adcData->array[0][7]<3200)
	{
		PointFlag++;
		FlagPoint=2;
		SetMotorDutyRatio(0.04,0.04);
		SysTickDelay(100);
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
		PointFlag=0;
	}
	else
	{
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
	}
	
}
void _ArrivePlatform(void)
{
		const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t temp=0;
	for(int i=0; i<=7; i++)
	{
		temp+=adcData->array[0][i];
	}
			//printf("1");
		if(IsLLaserChange() == Changed&&IsRLaserChange() == Changed)
			{
				for(int i;i<=7500;i++);
			if(temp<=22000)
			{
				//SysTickDelay(30);
				UpdateMotorState(MOTOR_STOP);
				//_FlagBack=1;
				_ResetUpDown();
				ClearLLaserChangePendingBit();
				ClearRLaserChangePendingBit();
			}
			}
	else 
	{
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
	}
		
		///
}
