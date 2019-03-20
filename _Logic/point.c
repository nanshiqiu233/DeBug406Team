#include "point.h"
static int _times=5;
static int _LastTimeButtomValue=0;
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
	
		_times++;
		return _LastTimeButtomValue;
	
}
void _FindPoint()
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t temp=0;
	_LastTimeButtomValue=_UpdateButtomValue();
	for(int i=0; i<=7; i++)
	{
		temp+=adcData->array[0][i];
	}
	if(_LastTimeButtomValue-temp>=3000&&(IsLLaserChange() == Changed||IsRLaserChange() == Changed))
	{

		SetMotorDutyRatio(0.06,0.06);
		SysTickDelay(200);
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
