#include "SHELL.h"
int Start=1;
int Stop=0;
uint8_t Command;
extern volatile uint8_t _Command[5];
void _Timer(void)
{
		_resetlaser();
		int i=0;
		i++;
		if(i==5)
		{
			PointFlag=0;
		}
}
void Match(void)
{
	printf("%d",Is_Command_Updated());
	if(Is_Command_Updated()==1&&Start==1)
	{
		Command=_Command[1];
		Start=0;
		Clear_CommandUpdate_PendingBit();
	}
	if(Start==0&&Stop==0)
	{
		if(Command==0x01)
		{
			TestOnly();
		}
	}
	if(Stop==1)
	{
		UpdateMotorState(MOTOR_STOP);
		printf("0xAB");
		Start=1;
		Stop=0;
	}
}
