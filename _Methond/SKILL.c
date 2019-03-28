#include "SKILL.h"
#define TEST2
int FlagUp=0;
int FlagDown=0;
int _FlagBack=0;
int FlagPoint=0;
int TurnFlag=0;
int FlagStop=0;
int FlagBalance=0;
void _resetlaser(void)
{
	if(_UpHillOrDownHillFeedBack()!=FlatGround||_IsBalanceFeedBack()!=BALANCE)
	{
		ClearLLaserChangePendingBit();
		ClearRLaserChangePendingBit();
	}
}
void InitFlag()
{
    FlagUp=0;
    FlagDown=0;
    _FlagBack=0;
		TurnFlag=0;
	 FlagUp=0;
 FlagDown=0;
 _FlagBack=0;
 FlagPoint=0;
 TurnFlag=0;
 FlagStop=0;
 FlagBalance=0;
}
void ThreeTOFour(void)
{
				if(FlagDown==0)
				{
					if(FlagUp==0)
						{
							if(_UpHillOrDownHillFeedBack()!=DOWN)
								{
									_GoLine();
									//printf("goline\r\n");
									if(_UpHillOrDownHillFeedBack()==UP)
										{
											FlagUp=1;
											_ArrivePlatform();
											//printf("arrive\r\n");
							
										}
									else if(_UpHillOrDownHillFeedBack()==FlatGround)
										{
											_FindPointGo();
											//printf("findpoint\r\n");
										}
								}
						else
								{
									FlagDown=1;
									_GoLineLowSpeed();
									//printf("lowspeed\r\n");
								}
						}
					else
						{
							if(UpgradeMotorState() == MOTOR_STOP)
								{
									//printf("low\r\n");
									Motor_TurnRightBlockingMode(180);
									SysTickDelay(200);
									UpdateMotorState(MOTOR_FRONT);
									_FlagBack=1;
									_GoLineLowSpeed();
									//printf("2");
						
								}
								else if(_FlagBack==1)
								{
									//printf("flagback\r\n");
									_GoLineLowSpeed();
									if(_UpHillOrDownHillFeedBack()== DOWN)
								{
										_FlagBack=0;
										FlagUp=0;
										//printf("down\r\n");
								}
								}
								else
								{
								_GoLine();
								//_ArrivePlatform();
								if(_UpHillOrDownHillFeedBack()==FlatGround)
								{
								UpdateMotorState(MOTOR_STOP);
								}
								//printf("7777777777777\r\n");
								}
						}
				
					
				}
			else
				{
					//printf("1\r\n");
					_GoLineLowSpeed();
					if(_UpHillOrDownHillFeedBack()==FlatGround)
					{
						FlagDown=0;
					}
				}
}
void OneTOTwo(void)
{

	if(FlagUp==0)
	{
		_FindPointGo();
		_GoLine();
		if(_UpHillOrDownHillFeedBack()== UP)
		{
					//printf("up1\r\n");
					FlagUp=1;

		}
	}
	else 
	{
		_FindPointGo();
		_GoLine();
		if(_UpHillOrDownHillFeedBack()==DOWN)
		{
			//printf("down1");
			FlagDown=1;
		}
	}
	if(FlagUp==1&&FlagDown==1)
	{
		if(_UpHillOrDownHillFeedBack()== UP)
		{
			//printf("up2");
			_FlagBack=1;
		}
	}
	if(_FlagBack==1)
	{
		if(_UpHillOrDownHillFeedBack()== FlatGround)
		{
		//printf("stop"); 
		UpdateMotorState(MOTOR_STOP);
		}
	}
}
void TwoTOThree(void)
{
	if(UpgradeMotorState() == MOTOR_STOP&&FlagStop!=2)
	{
		UpdateMotorState(MOTOR_FRONT);
		FlagStop++;
		SetMotorDutyRatio(0.04,0.04);
		SysTickDelay(500);
		//FlagStop++;
	}
	if(FlagStop==1)
	{
		_GoLine();
		if(FlagDown==0)
			{
		if(_IsBalanceFeedBack()==UNBALANCE)
		{
			FlagBalance=1;
		}
		if(FlagBalance==1)
			{
		if(_UpHillOrDownHillFeedBack()==DOWN&&_IsBalanceFeedBack()==BALANCE)
		{
			FlagDown=1;
			//SysTickDelay(1000);
		}
			}
		}
		else if(FlagDown==1)
		{
			_GoLine();
			printf("flagdown\r\n");
			if(_UpHillOrDownHillFeedBack()==FlatGround&&_IsBalanceFeedBack()==BALANCE)
			{
				_GoLine();
				printf("and\r\n");
				_FindPointStop();
				if(UpgradeMotorState() == MOTOR_STOP)
				{
					SysTickDelay(100);
					Motor_TurnRightBlockingMode(151);
					//Motor_TurnToAbsoluteYawAngle(0);
					FlagDown=0;
				}
			}
		}
			}
	else if(FlagStop==2)
	{
		_GoLine();
		_FindPointGo();
		if(_UpHillOrDownHillFeedBack()==UP)
		{
			FlagUp=1;	
		}
		if(FlagUp==1)
			{
				if(_UpHillOrDownHillFeedBack()==FlatGround)
			  {
					UpdateMotorState(MOTOR_STOP);
				}
			}
	}
	else if(FlagStop==0)
	{
	if(_FlagBack==0)
	{
		Motor_TurnRightBlockingMode(180);
		UpdateMotorState(MOTOR_FRONT);
		_FlagBack=1;
	}
	else if(_FlagBack==1)
	{
		_GoLineLowSpeed();
		if(_UpHillOrDownHillFeedBack()== DOWN)
		{
			FlagDown=1;
		}
	}
		if(FlagDown==1)
		{
			if(_UpHillOrDownHillFeedBack()==FlatGround)
			{
				_GoLine();
				_FindPointStop();
				if(UpgradeMotorState() == MOTOR_STOP)
				{
					Motor_TurnRightBlockingMode(35);
					FlagDown=0;
					//SetMotorDutyRatio(0.04,0.04);
					//SysTickDelay(100);
				}
			}
			else if(_UpHillOrDownHillFeedBack()==DOWN)
			{
				_GoLineLowSpeed();
			}
		}
	}
}

void TwoTOFour()
{
	if(UpgradeMotorState() == MOTOR_STOP&&FlagStop!=2)
	{
		UpdateMotorState(MOTOR_FRONT);
		FlagStop++;
		SetMotorDutyRatio(0.04,0.04);
		SysTickDelay(500);
		//FlagStop++;
	}
	if(FlagStop==1)
	{
		_GoLine();
		if(FlagDown==0)
			{
		if(_IsBalanceFeedBack()==UNBALANCE)
		{
			FlagBalance=1;
		}
		if(FlagBalance==1)
			{
		if(_UpHillOrDownHillFeedBack()==DOWN&&_IsBalanceFeedBack()==BALANCE)
		{
			FlagDown=1;
			//SysTickDelay(1000);
		}
			}
		}
		else if(FlagDown==1)
		{
			_GoLine();
			//printf("flagdown\r\n");
			if(_UpHillOrDownHillFeedBack()==FlatGround&&_IsBalanceFeedBack()==BALANCE)
			{
				_GoLine();
				//printf("and\r\n");
				_FindPointStop();
				if(UpgradeMotorState() == MOTOR_STOP)
				{
					SysTickDelay(100);
					Motor_TurnLeftBlockingMode(30);
					//Motor_TurnToAbsoluteYawAngle(0);
					FlagDown=0;
				}
			}
		}
			}
	else if(FlagStop==2)
	{
		_GoLine();
		_FindPointGo();
		if(_UpHillOrDownHillFeedBack()==UP)
		{
			FlagUp=1;	
		}
		if(FlagUp==1)
			{
				if(_UpHillOrDownHillFeedBack()==FlatGround)
			  {
					UpdateMotorState(MOTOR_STOP);
				}
			}
	}
	else if(FlagStop==0)
	{
	if(_FlagBack==0)
	{
		Motor_TurnRightBlockingMode(180);
		UpdateMotorState(MOTOR_FRONT);
		_FlagBack=1;
	}
	else if(_FlagBack==1)
	{
		_GoLineLowSpeed();
		if(_UpHillOrDownHillFeedBack()== DOWN)
		{
			FlagDown=1;
		}
	}
		if(FlagDown==1)
		{
			if(_UpHillOrDownHillFeedBack()==FlatGround)
			{
				_GoLine();
				_FindPointStop();
				if(UpgradeMotorState() == MOTOR_STOP)
				{
					Motor_TurnRightBlockingMode(40);
					FlagDown=0;
					//SetMotorDutyRatio(0.04,0.04);
					//SysTickDelay(100);
				}
			}
			else if(_UpHillOrDownHillFeedBack()==DOWN)
			{
				_GoLineLowSpeed();
			}
		}
	}
}
