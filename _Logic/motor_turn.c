#include "motor_turn.h"


void Motor_TurnLeftBlockingMode(float Angle)
{
  float CurrentAngle = 0;
	float TurnAngle = 0;
	
	SysTickDelay(200);
	
	CurrentAngle = Gyro_GetYawAngle();
	
	if(Angle > CurrentAngle)
	{
		TurnAngle = Angle + CurrentAngle;
		UpdateMotorState(MOTOR_TURN_LEFT);
		SetMotorDutyRatio(0.1,0.1);
		
		while(fabs(TurnAngle - Gyro_GetYawAngle()) > 10)
		{
		}
		UpdateMotorState(MOTOR_STOP);
		SetMotorDutyRatio(0,0);
		
		
	}
	else if(Angle < CurrentAngle)
	{
		
		TurnAngle = Angle + CurrentAngle;
		
		if(TurnAngle > 360)
		{
			TurnAngle -= 360;
			
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(0.1,0.1);
			
			while(Gyro_GetYawAngle() < 357)
			{
			}
			SysTickDelay(70);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 10)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);
			
		}
		else
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(0.1,0.1);
			
			while(fabs(TurnAngle - Gyro_GetYawAngle()) > 10)
			{
			}
			UpdateMotorState(MOTOR_STOP);
			SetMotorDutyRatio(0,0);			
			
		}
	}
	
	SysTickDelay(100);
	
	//微调
	
	while(fabs(TurnAngle - Gyro_GetYawAngle()) > 3)
	{
		if(TurnAngle > Gyro_GetYawAngle())
		{
			UpdateMotorState(MOTOR_TURN_LEFT);
			SetMotorDutyRatio(0.06,0.06);
		}
		
		if(Gyro_GetYawAngle() > TurnAngle)
		{
			UpdateMotorState(MOTOR_TURN_RIGHT);
			SetMotorDutyRatio(0.06,0.06);
		}
	}
	
	
	UpdateMotorState(MOTOR_STOP);
	SetMotorDutyRatio(0,0);			
	
}


void Motor_TurnRightBlockingMode(float Angle)
{
  float CurrentAngle = 0;
  float TurnAngle = 0;
	
	SysTickDelay(200);

  CurrentAngle = Gyro_GetYawAngle();

  if(Angle > CurrentAngle)
    {
      TurnAngle = (float)360.0 - (float)(Angle - CurrentAngle);
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(0.1,0.1);

      while(Gyro_GetYawAngle() > (float)2)
        {
        }
      SysTickDelay(100);

      while(fabs(Gyro_GetYawAngle() - TurnAngle) > 10)
        {
        }
    }
  else if(Angle <= CurrentAngle)
    {
      TurnAngle = CurrentAngle - Angle;
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(0.1,0.1);
      while(fabs(Gyro_GetYawAngle() - TurnAngle) > 10)
        {
        }
    }


  UpdateMotorState(MOTOR_STOP);
  SetMotorDutyRatio(0,0);
  SysTickDelay(200);

  while(fabs((double)(TurnAngle - Gyro_GetYawAngle())) > 2)
    {
      if(TurnAngle - Gyro_GetYawAngle() > (float)2)
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(0.06,0.06);
        }
      else if(Gyro_GetYawAngle() - TurnAngle > (float)2)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
          SetMotorDutyRatio(0.06,0.06);
        }
    }

  UpdateMotorState(MOTOR_STOP);
  SetMotorDutyRatio(0,0);

}


void Motor_TurnToAbsoluteYawAngle(float aimAngle)
{
  float currentAngle = Gyro_GetYawAngle(),
        distanceAngle = currentAngle - aimAngle,
        distanceAngle2;

  if(distanceAngle > 0)
    {
      distanceAngle2 = aimAngle + 360 - currentAngle;
      if((distanceAngle - distanceAngle2 ) < 0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
        }
      else
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
        }
    }
  else
    {
      distanceAngle2 = aimAngle - 360 - currentAngle;
      if((distanceAngle - distanceAngle2 ) < 0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
        }
      else
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
        }
    }

  SetMotorDutyRatio(0.1, 0.1);

  while(fabs(Gyro_GetYawAngle() - aimAngle) > 2.0)
    {
    }

  SysTickDelay(100);


  while(fabs(Gyro_GetYawAngle() - aimAngle) > 2.0)
    {
      if(aimAngle - Gyro_GetYawAngle() > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(0.05,0.05);
        }
      else if(Gyro_GetYawAngle() - aimAngle > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
          SetMotorDutyRatio(0.05,0.05);
        }

    }


  SetMotorDutyRatio(0,0);
  UpdateMotorState(MOTOR_STOP);

}
