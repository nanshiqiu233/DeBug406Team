#include "point.h"
void _FindPoint()
{
	const AdcData_t * adcData;
	adcData = UpdateButtom();
	uint32_t temp=0;
	for(int i=0; i<=7; i++)
	{
		temp+=adcData->array[0][i];
	}
	if(temp<=20114&&(IsLLaserChange() == Changed||IsRLaserChange() == UnChange))
	{
		UpdateMotorState(MOTOR_STOP);
	}
	/*test only*/
	//adcData->array[0][i];
	printf("sum=%d\r\n",temp);
	printf("%d\r\n",IsLLaserChange());
	printf("%d\r\n",IsRLaserChange());
	/*test only*/
}
