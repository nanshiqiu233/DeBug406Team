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
	//printf("temp=%d\r\n",temp);
	//printf("last=%d\r\n",_LastTimeButtomValue);
	if(_LastTimeButtomValue-temp>=8000&&(IsLLaserChange() == Changed||IsRLaserChange() == Changed))
	{
		//UpdateMotorState(MOTOR_STOP);
		UpdateMotorState(MOTOR_FRONT);
		SetMotorDutyRatio(0.07,0.07);
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
		//printf("1");
		SysTickDelay(75);
	}
	/*test only*/
	//adcData->array[0][i];
//	printf("sum=%d\r\n",temp);
//	printf("%d\r\n",IsLLaserChange());
//	printf("%d\r\n",IsRLaserChange());
	/*test only*/
	
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
	if(IsLLaserChange() == Changed&&IsRLaserChange() == Changed)
		{
			SysTickDelay(125);
	if(temp<=20000)
	{
		//SysTickDelay(30);
		UpdateMotorState(MOTOR_STOP);
	}
		}
	else 
	{
			ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
	}
}
