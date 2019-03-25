#include "motor_turn.h"


void Motor_TurnLeftBlockingMode(float Angle)
{
  float CurrentAngle = 0;


  CurrentAngle = Gyro_GetYawAngle();

  if(((float)(CurrentAngle + Angle)) > (float)360.0 )
    {
      Angle += CurrentAngle;
      Angle -= (float)360.0;

      UpdateMotorState(MOTOR_TURN_LEFT);
      SetMotorDutyRatio(0.1,0.1);

      while(Gyro_GetYawAngle() < (float)355.0)
        {
        }
      SysTickDelay(100);
      while(Gyro_GetYawAngle() < Angle)
        {
        }

      SetMotorDutyRatio(0.0,0.0);
      UpdateMotorState(MOTOR_STOP);

      SysTickDelay(100);

    }
  else
    {
      Angle += CurrentAngle;
      UpdateMotorState(MOTOR_TURN_LEFT);
      SetMotorDutyRatio(0.1,0.1);

      while(Gyro_GetYawAngle() < Angle)
        {
        }

      SetMotorDutyRatio(0.0,0.0);
      UpdateMotorState(MOTOR_STOP);
			SysTickDelay(100);

    }
		

  while(fabs((double)(Angle - Gyro_GetYawAngle())) > 2.0)
    {
      if(Angle - Gyro_GetYawAngle() > (float)2.0)
        {
					//19.3.25
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(0.06,0.06);
					
        }
      else if(Gyro_GetYawAngle() - Angle > 2)
        {
          UpdateMotorState(MOTOR_TURN_RIGHT);
          SetMotorDutyRatio(0.06,0.06);
					
        }
		}

  SetMotorDutyRatio(0.0,0.0);
  UpdateMotorState(MOTOR_STOP);



}


void Motor_TurnRightBlockingMode(float Angle)
{
  float CurrentAngle = 0;
  float TurnAngle = 0;

  CurrentAngle = Gyro_GetYawAngle();

  if(Angle > CurrentAngle)
    {
      TurnAngle = (float)360.0 - (float)(Angle - CurrentAngle);
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(0.1,0.1);

      while(Gyro_GetYawAngle() > (float)3.0)
        {
        }
      SysTickDelay(100);

      while(TurnAngle < Gyro_GetYawAngle())
        {
        }
    }
  else if(Angle <= CurrentAngle)
    {
      TurnAngle = CurrentAngle - Angle;
      UpdateMotorState(MOTOR_TURN_RIGHT);
      SetMotorDutyRatio(0.1,0.1);
      while(TurnAngle < Gyro_GetYawAngle())
        {
        }
    }


  UpdateMotorState(MOTOR_STOP);
  SetMotorDutyRatio(0,0);
  SysTickDelay(100);

  while(fabs((double)(TurnAngle - Gyro_GetYawAngle())) > 2.0)
    {
      if(TurnAngle - Gyro_GetYawAngle() > (float)2.0)
        {
          UpdateMotorState(MOTOR_TURN_LEFT);
          SetMotorDutyRatio(0.06,0.06);
        }
      else if(Gyro_GetYawAngle() - TurnAngle > (float)2.0)
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
