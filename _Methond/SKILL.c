#include "SKILL.h"

int FlagUp=0;
int FlagDown=0;
int _FlagBack=0;
int FlagPoint=0;
int TurnFlag=0;
void InitFlag()
{
    FlagUp=0;
    FlagDown=0;
    _FlagBack=0;
		TurnFlag=0;
}
void ThreeTOFour(void)
{
				for(int i=1;i<=1000;i--)
				{
					_GoLineLowSpeed();
				}
	
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
void TwoTOThree()
{
	if(_FlagBack==0)
	{
	Motor_TurnRightBlockingMode(180);
	_FlagBack=1;
	SetMotorDutyRatio(0.0,0.0);
	//UpdateMotorState(MOTOR_FRONT);
	SysTickDelay(2000);
	UpdateMotorState(MOTOR_FRONT);
	}
	/*
	需要加入举手和动头。
	*/
	if(FlagDown==0)
		{
			_GoLineLowSpeed();
			if(_UpHillOrDownHillFeedBack()==DOWN)
				{
					FlagDown=1;
				}
		}
	if(FlagDown==1)
	{
		if(FlagPoint==0)
		{
			if(_UpHillOrDownHillFeedBack()==FlatGround)
				{
					_GoLine();
					_FindPointStop();
				}
		}
		if(FlagPoint==1)
		{
			Motor_TurnRightBlockingMode(40);
			
			//UpdateMotorState(MOTOR_FRONT);
			SetMotorDutyRatio(0.0,0.0);
			UpdateMotorState(MOTOR_FRONT);
			SysTickDelay(2000);
			SetMotorDutyRatio(0.14,0.14);
			SysTickDelay(200);
			TurnFlag=1;
		}
		if(TurnFlag==1)
		{
			_GoLineLowSpeed();
			if(_UpHillOrDownHillFeedBack()==FlatGround)
			{
				_FindPointStop();
				if(UpgradeMotorState() == MOTOR_STOP)
				{
					Motor_TurnRightBlockingMode(140);
					SetMotorDutyRatio(0.0,0.0);
					UpdateMotorState(MOTOR_FRONT);
					SysTickDelay(2000);
					TurnFlag=2;
				}
			}

		}
		if(TurnFlag==2)
		{
			_GoLine();
			_FindPointGo();
			if(_UpHillOrDownHillFeedBack()==UP)
			{
				FlagUp=1;
			}
		}
		
	}
	if(FlagDown==1&&FlagUp==1&&TurnFlag==2)
	{
		if(_UpHillOrDownHillFeedBack()==FlatGround)
		{
			UpdateMotorState(MOTOR_STOP);
		}
	}
}