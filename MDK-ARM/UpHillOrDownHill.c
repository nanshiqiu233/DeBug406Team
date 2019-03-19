#include "UpHillOrDownHill.h"
//#define NegativeNumber
static UpHillOrDownHill _UpOrDown=FlatGround;
static float PrimaryRollAngle=0;

UpHillOrDownHill _UpHillOrDownHillFeedBack()
{
	printf("1=%f\r\n",1.0*Gyro_GetPitchAngle());
	return _UpOrDown;
}
void _UpHillOrDownHillCheck(void)
{
	#ifdef NegativeNumber

	if((Gyro_GetPitchAngle()-PrimaryRollAngle<-2)&&(Gyro_GetPitchAngle()-PrimaryRollAngle>-100))
	{
		_UpOrDown=UP;
	}

	else if((Gyro_GetPitchAngle()-PrimaryRollAngle<-300)&&(Gyro_GetPitchAngle()-PrimaryRollAngle>-200))
	{
		_UpOrDown=DOWN;
	}
		else if((Gyro_GetPitchAngle()-PrimaryRollAngle>-1)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<1))
	{
		_UpOrDown=FlatGround;
	}
	#else
		if((Gyro_GetPitchAngle()-PrimaryRollAngle>300)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<357))
	{
		_UpOrDown=UP;
	}

	else if((Gyro_GetPitchAngle()-PrimaryRollAngle>2)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<100))
	{
		_UpOrDown=DOWN;
	}
	else if(((Gyro_GetPitchAngle()-PrimaryRollAngle>-1)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<1))||((Gyro_GetPitchAngle()-PrimaryRollAngle>358)&&(Gyro_GetPitchAngle()-PrimaryRollAngle<360)))
	{
		_UpOrDown=FlatGround;
	}
	#endif
}
void _GetPrimaryRollAngle(void)
{
	PrimaryRollAngle=Gyro_GetPitchAngle();
}
void _ResetUpDown(void)
{
	_UpOrDown=FlatGround;
}
