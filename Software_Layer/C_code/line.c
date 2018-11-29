/** See a brief introduction (right-hand button) */
#include "line.h"
#include "math.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RESET   0
#define SET     1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const int32_t _HIGH_E_LEVEL   = 2900;
static const int32_t _LOW_E_LEVEL    = 700;

/* ADC message */
static int _zeroNum[2];
static int _nagtiveNum[2];
static double _middleLineOffset[2];

/* Recognize arguments */
static uint8_t _pointRecognizeAsLine[2] = {2, 2};
static uint8_t _pointRecognizeAsStop[2] = {9, 9};

/* Speed control */
static int32_t _baseSpeed = 10000;
static int32_t _straightSpeedCorrection = 0;
static int32_t _skewingSpeedCorrection = 0;
static int32_t _upperHillSpeedCorrection = 0;
static uint8_t _transSpeed = 20;

/* Trigger use */
static uint16_t _triggerDelay = 0;
static uint8_t _pointNumber = 2;
static uint8_t _delayMs = 0;

/* Private function prototypes -----------------------------------------------*/
void _BinaryzationADC(void);
void _GoStraightCorrection(void);
void _PIDSpeedCorrection(double nowErr);
void _PIDSpeedCircleCorrection(double nowErr);
void _PIDSpeedCorrection_New(double nowErr, double intErr);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void GoOnLine(void)
{
  static double correctionRange = 0.25;
  double tmpLineOffset[2];
	
  /* No-Line Default */
  if(_middleLineOffset[0] < 0)
  {
    _GoStraightCorrection(); 
  }
  else if((_zeroNum[0] + _nagtiveNum[0]) <= _pointRecognizeAsLine[0] &&
          (_zeroNum[1] + _nagtiveNum[1]) <= _pointRecognizeAsLine[1])
  {
		_middleLineOffset[0] -= 9;
    tmpLineOffset[0] =  _middleLineOffset[0] / 6.5;
    if( _middleLineOffset[1] == -1)
    {
      _middleLineOffset[1] = 0.25;
    }
    else
    {
			_middleLineOffset[1] -= 9;
			tmpLineOffset[1] =  _middleLineOffset[1] / 7;
    }
    
    if(_middleLineOffset[0] > correctionRange)
    {
      (_middleLineOffset[0] >= _middleLineOffset[1] - 0.5) ?
      _PIDSpeedCorrection( sqrt( tmpLineOffset[0])) :
      _PIDSpeedCorrection(-sqrt( tmpLineOffset[1])) ;
    }
    else if(_middleLineOffset[0] < correctionRange)
    {
      (_middleLineOffset[0] <= _middleLineOffset[1] + 0.5) ?
      _PIDSpeedCorrection(-sqrt(-tmpLineOffset[0])) :
      _PIDSpeedCorrection( sqrt(-tmpLineOffset[1])) ;
    }
    else //_middleLineOffset[0] == 0
    {
			 _GoStraightCorrection();       
    }
  }
  
  /* Stop Detection */
  if((_zeroNum[0] + _nagtiveNum[0]) >= _pointRecognizeAsStop[0] || 
     (_zeroNum[1] + _nagtiveNum[1]) >= _pointRecognizeAsStop[1])
  {
    UpdateMotorState(MOTOR_STOP);
  }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void GoOnCircle(void)
{
    static double correctionRange = 0.25;
  double tmpLineOffset[2];
	
  /* No-Line Default */
  if((_zeroNum[0] + _nagtiveNum[0]) <= _pointRecognizeAsLine[0] &&
     (_zeroNum[1] + _nagtiveNum[1]) <= _pointRecognizeAsLine[1])
  {
		_middleLineOffset[0] -= 9;
    tmpLineOffset[0] =  _middleLineOffset[0] / 6.5;
    if( _middleLineOffset[1] == -1)
    {
      _middleLineOffset[1] = 0.25;
    }
    else
    {
			_middleLineOffset[1] -= 9;
			tmpLineOffset[1] =  _middleLineOffset[1] / 7;
    }
    
    if(_middleLineOffset[0] > correctionRange)
    {
      (_middleLineOffset[0] >= _middleLineOffset[1] - 0.5) ?
      _PIDSpeedCorrection( sqrt( tmpLineOffset[0])) :
      _PIDSpeedCorrection(-sqrt( tmpLineOffset[1])) ;
    }
    else if(_middleLineOffset[0] < correctionRange)
    {
      (_middleLineOffset[0] <= _middleLineOffset[1] + 0.5) ?
      _PIDSpeedCorrection(-sqrt(-tmpLineOffset[0])) :
      _PIDSpeedCorrection( sqrt(-tmpLineOffset[1])) ;
    }
    else //_middleLineOffset[0] == 0
    {
			 _GoStraightCorrection();       
    }
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void UphillGoStraight(void)
{
	if(_upperHillSpeedCorrection == 0)
	{
		_upperHillSpeedCorrection = - _skewingSpeedCorrection;
	}
	
	if(GetRFSpeed(1) > GetLRSpeed(1))
  {
    _upperHillSpeedCorrection -= (-100);
  }
  else if(GetRFSpeed(1) < GetLRSpeed(1))
  {
    _upperHillSpeedCorrection -= (+100);
  }
	
  /* Set Speed */
  SetMotorPulse(_baseSpeed + _upperHillSpeedCorrection, 
                _baseSpeed - _upperHillSpeedCorrection);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _PIDSpeedCorrection(double nowErr)
{
  static double const kP = 1, kI = 0, kD = 0;
  static double const integralThreshold = 20;
  static double       preErr = 0, integralErr = 0;

  integralErr += nowErr;
  if (integralErr >  integralThreshold) integralErr =  integralThreshold;
  if (integralErr < -integralThreshold) integralErr = -integralThreshold;
  _skewingSpeedCorrection = _baseSpeed * ( /* PID argument can not upper than 1 */
                            kP *  nowErr + 
                            kI *  integralErr + 
                            kD * (nowErr - preErr)) - 1;
  preErr = nowErr;
  
  /* Set Speed */
  SetMotorPulse(_baseSpeed - _skewingSpeedCorrection, 
                _baseSpeed + _skewingSpeedCorrection);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _PIDSpeedCircleCorrection(double nowErr)
{
  static double const kP = 0.6, kI = 0.6, kD = 0;
  static double const integralThreshold = 2;
  static double       preErr = 0, integralErr = 0;

  integralErr += nowErr;
  if (integralErr >  integralThreshold) integralErr =  integralThreshold;
  if (integralErr < -integralThreshold) integralErr = -integralThreshold;
  _skewingSpeedCorrection = _baseSpeed * ( /* PID argument can not upper than 1 */
                            kP *  nowErr + 
                            kI *  integralErr + 
                            kD * (nowErr - preErr)) - 1;
  preErr = nowErr;
  
  /* Set Speed */
  SetMotorPulse(_baseSpeed - _skewingSpeedCorrection, 
                _baseSpeed + _skewingSpeedCorrection);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void GoStraight(void)
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
int PointRecognition(void)
{
  static int tag = 0, pointN = 0;
  
	if(_triggerDelay > 0 || _nagtiveNum[0] == 0)
	{
		tag = 0;
	}
  else if(tag < 3 && _nagtiveNum[0] >= 3)
  {
    ++tag;
  }
  else if(tag == 3 && _nagtiveNum[0] <= 1)
  {
    tag = 0;
    ++pointN;
		_triggerDelay = 700;
  }
  
	if(pointN >= _pointNumber) 
	{
    tag = 0;
		pointN = 0;
		return 1;
	}
	
  return 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SetTriggerDelay(uint16_t ms)
{
	_triggerDelay = ms;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t GetTriggerDelay(void)
{
	return _triggerDelay;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int PitchRecognition(void)
{
  return 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void GoOnLine_New(void)
{    
  /* No-Line Default */
  if(_middleLineOffset[0] < 0)
  {

  }
  /* Front adc */
  else if((_zeroNum[0] + _nagtiveNum[0]) <= _pointRecognizeAsLine[0])
  {
    _middleLineOffset[0] -= 9;
    _middleLineOffset[0] /= 7;
    
    _middleLineOffset[0] > 0 ? 
    (_middleLineOffset[0] =  sqrt( _middleLineOffset[0])): 
    (_middleLineOffset[0] = -sqrt(-_middleLineOffset[0]));

     _middleLineOffset[1] == -1 ?
    (_middleLineOffset[1] = 0) : 
    (
      _middleLineOffset[1] -= 9,
      _middleLineOffset[1] /= 7,
    
       _middleLineOffset[1] > 0 ? 
      (_middleLineOffset[1] = -sqrt( _middleLineOffset[1])): 
      (_middleLineOffset[1] =  sqrt(-_middleLineOffset[1]))
    );
    
    _PIDSpeedCorrection_New(_middleLineOffset[0], _middleLineOffset[1]);
  }
  
  /* Stop Detection */
  if((_zeroNum[0] + _nagtiveNum[0]) >= _pointRecognizeAsStop[0] || 
     (_zeroNum[1] + _nagtiveNum[1]) >= _pointRecognizeAsStop[1])
  {
    UpdateMotorState(MOTOR_STOP);
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int GetZeroPointNumber(int place)
{
  return _zeroNum[place];
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int GetNagtivePointNumber(int place)
{
  return _nagtiveNum[place];
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint32_t GetDelayMs(void)
{
  return _delayMs * 10;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SetBaseSpeed(int32_t baseSpeed)
{
	_baseSpeed = baseSpeed;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _PIDSpeedCorrection_New(double nowErr, double intErr)
{
  static double const kP = 0.8, kI = 0.01, kD = 0;
  static double const integralThreshold = 20;
  static double       preErr = 0, integralErr = 0;

  integralErr += intErr;
  if (integralErr >  integralThreshold) integralErr =  integralThreshold;
  if (integralErr < -integralThreshold) integralErr = -integralThreshold;
  _skewingSpeedCorrection = _baseSpeed * ( /* PID argument can not upper than 1 */
                            kP *  nowErr + 
                            kI *  integralErr + 
                            kD * (nowErr - preErr));
  preErr = nowErr;
  
  /* Set Speed */
  SetMotorPulse(_baseSpeed - _skewingSpeedCorrection, 
                _baseSpeed + _skewingSpeedCorrection);
  
#ifdef _USE_SERIAL_DEBUG  
  printf("integralErr:%f\r\n", integralErr);
#endif //!_USE_SERIAL_DEBUG
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _GoStraightCorrection(void)
{
  if(GetLRSpeed(1) < GetRFSpeed(1))
  {
    _skewingSpeedCorrection += 10;
  }
  else if(GetLRSpeed(1) > GetRFSpeed(1))
  {
    _skewingSpeedCorrection -= 10;
  }
  else if(_straightSpeedCorrection != 0)
  {
    _straightSpeedCorrection = _skewingSpeedCorrection;
  }
  
  /* Set Speed */
  SetMotorPulse(_baseSpeed - _skewingSpeedCorrection, 
                _baseSpeed + _skewingSpeedCorrection);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void ADCUpdate(void)
{
  const AdcData_t * _adcDataPoint = UpdateButtom();
  
  for(int row = 0; row < 2; ++row)
  {
    /* Init */
    int fromLeftBit = 0;
    _zeroNum[row] = 0;
    _nagtiveNum[row] = 0;
    _middleLineOffset[row] = -1;

    /* Quick Scan */
    for(fromLeftBit = 0; fromLeftBit < 8; ++fromLeftBit)
    {
      if(_adcDataPoint->array[row][fromLeftBit] >= _HIGH_E_LEVEL)
      {
        continue;
      }
      else if(_adcDataPoint->array[row][fromLeftBit] <= _LOW_E_LEVEL)
      {
        ++_nagtiveNum[row];
      }
      else
      {
        ++_zeroNum[row];
      }
    }
    
    /* Matching Middle Line */
    if((_nagtiveNum[row] + _zeroNum[row]) > 0){
      for(fromLeftBit = 0; fromLeftBit < 8; ++fromLeftBit)
      {
        if(_adcDataPoint->array[row][fromLeftBit] >= _HIGH_E_LEVEL)
        {
          continue;
        }
        else if(_adcDataPoint->array[row][fromLeftBit] <= _LOW_E_LEVEL)
        {
          _middleLineOffset[row] += (fromLeftBit + 1.5/* + 1 + 1 - 0.5*/);
          break;
        }
        else
        {
          _middleLineOffset[row] += (fromLeftBit + 2  /* + 1 + 1*/);
          break;
        }        
      }
      
      for(int fromRightBit = 7; fromRightBit >= 0; --fromRightBit)
      {
        if(_adcDataPoint->array[row][fromRightBit] >= _HIGH_E_LEVEL)
        {
          continue;
        }
        else if(_adcDataPoint->array[row][fromRightBit] <= _LOW_E_LEVEL)
        {
          _middleLineOffset[row] += (fromRightBit + 1.5/* + 1 + 0.5*/);
          break;
        }
        else
        {
          _middleLineOffset[row] += (fromRightBit + 1  /* + 1*/);
          break;
        }
      }
    }
  }

#ifdef _USE_SERIAL_DEBUG  
  for(int i = 0; i < 2; ++i)
  {
    printf("0:%d -1:%d m:%f\r\n", _zeroNum[i], _nagtiveNum[i], _middleLineOffset[i]);
  }
#endif //!_USE_SERIAL_DEBUG
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _ArgumentsUpdate_Callback(uint8_t args[])
{
  _pointNumber = args[0];
  _delayMs = args[1];
  _transSpeed = args[2];
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t GetTransSpeed(void)
{
  if(_transSpeed > 0 && _transSpeed < 200)
    return _transSpeed * 100;
  return 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TestGoStraight(void)
{
	static uint32_t tmpSpeed = 0;
	if(GetRFSpeed(1) > GetLRSpeed(1))
  {
    tmpSpeed += 10;
  }
  else if(GetRFSpeed(1) < GetLRSpeed(1))
  {
    tmpSpeed -= 10;
  }
	
  /* Set Speed */
  SetMotorPulse(1400 - tmpSpeed, 
                1400 + tmpSpeed);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _PointDelay_Interruput(void)
{
	if(_triggerDelay > 0)
	{
		--_triggerDelay;
	}
}
