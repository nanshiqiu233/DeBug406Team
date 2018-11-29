/** See a brief introduction (right-hand button) */
#include "point.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _TRIGGER 1
#define _NOT_TRG 0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static const short _gradientThreshold = 5000;
//static const short _vibrationThreshold = 2000;
static int32_t _baseSpeed = 1200;
static int32_t _straightSpeedCorrection = 0;
static int     _highLowPlace = 1;// High Place Default 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init transition mode speed
  * @param  None
  * @retval None
  */
void TransitionSpeedReform(void)
{
  //TODO: Go Straight with low speed
  SetMotorPulse(_GOSTRAIGHT_SPEEDLEFT, _GOSTRAIGHT_SPEEDRIGHT);
  UpdateMotorState(MOTOR_FRONT);
}

/**
  * @brief  Transition Mode Check A Start-Tag 
  * @param  None
  * @retval None
  */
int CheckStartPoint(void)
{
	static int tag = 0;
  
  if(_highLowPlace == 0 && 
    (GetNagtivePointNumber(0) + GetZeroPointNumber(0)) == 2)
  {
		SetTriggerDelay(500);//1s Point & Transition not trigger
    return _TRIGGER;
  }
  /* _highLowPlace == 1 */
  else if(tag == 0 && GetRollAngle() > 18)
	{
		tag = 1;
	}
	else if(tag == 1 && GetRollAngle() < 8)
  {
    _highLowPlace = tag = 0;
    
    return _TRIGGER;
  }
    
  return _NOT_TRG;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int CheckEndPointCheckIn(void)
{
	static int tag = 0;
	
  if(GetRollAngle() < -25 && (GetNagtivePointNumber(0) + GetZeroPointNumber(0)) != 0)
	{
		_highLowPlace = 1;
		return _TRIGGER;
	}
  else if(/*GetRollAngle() > -2 && GetRollAngle() < 2 &&*/
					GetNagtivePointNumber(0) >= 3 &&
					GetTriggerDelay() == 0)
  {
		++tag;
		if(tag >= 3)
		{
			tag = 0;
			return _TRIGGER;
		}
  }
	else
	{
		tag = 0;
	}
    
  return _NOT_TRG;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void GoStraightCorrection_TransitionMode(void)
{
  if(GetRFSpeed(1) > GetLRSpeed(1))
  {
    _straightSpeedCorrection += 10;
  }
  else if(GetRFSpeed(1) < GetLRSpeed(1))
  {
    _straightSpeedCorrection -= 10;
  }
  
  /* Set Speed */
  SetMotorPulse(_baseSpeed - _straightSpeedCorrection, 
                _baseSpeed + _straightSpeedCorrection);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */   
int GetHighLowPlaceTag(void)
{
  return _highLowPlace;
}
