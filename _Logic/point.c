#include "point.h"
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
void _FindPoint()
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t temp=0;
	for(int i=0; i<=7; i++)
	{
		temp+=adcData->array[0][i];
	}
	if(temp<=20114&&(IsLLaserChange() == Changed||IsRLaserChange() == Changed))
	{
		//UpdateMotorState(MOTOR_STOP);
		UpdateMotorState(MOTOR_FRONT);
		SetMotorDutyRatio(0.14,0.14);
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
		//printf("1");
		SysTickDelay(50);
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
			SysTickDelay(100);
	if(temp<=22000)
	{
		SysTickDelay(50);
		UpdateMotorState(MOTOR_STOP);
	}
		}
	else 
	{
			ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
	}
}
