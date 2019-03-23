#include "SKILL.h"
int FlagUp=0;
int FlagDown=0;
int _FlagBack=0;
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
											_FindPoint();
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
