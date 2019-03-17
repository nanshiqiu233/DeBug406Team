#include "UpHillOrDownHill.h"
static UpHillOrDownHill _UpOrDown=FlatGround;
static float PrimaryRollAngle=0;
//static int time=1;
UpHillOrDownHill _UpHillOrDownHillFeedBack()
{
	//printf("1=%f\r\n",1.0*(Gyro_GetPitchAngle()-PrimaryRollAngle));
	return _UpOrDown;
}
void _UpHillOrDownHillCheck(void)
{

//	if(Gyro_GetPitchAngle()-PrimaryRollAngle<-5&&(Gyro_GetPitchAngle()-PrimaryRollAngle>-100))
//	{
//		_UpOrDown=UP;
//	}

//	if(Gyro_GetPitchAngle()-PrimaryRollAngle<-300&&(Gyro_GetPitchAngle()-PrimaryRollAngle>-200))
//	{
//		_UpOrDown=DOWN;
//	}
		if(Gyro_GetPitchAngle()-PrimaryRollAngle>300&&(Gyro_GetPitchAngle()-PrimaryRollAngle<360))
	{
		_UpOrDown=UP;
	}

	if(Gyro_GetPitchAngle()-PrimaryRollAngle>2&&(Gyro_GetPitchAngle()-PrimaryRollAngle<100))
	{
		_UpOrDown=DOWN;
	}
}
void _GetPrimaryRollAngle(void)
{
	PrimaryRollAngle=Gyro_GetPitchAngle();
}
void _ResetUpDown(void)
{
	_UpOrDown=FlatGround;
}
