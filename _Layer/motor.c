/** See a brief introduction (right-hand button) */
#include "motor.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const static uint32_t _timerClockAPB1 = 84000000;
const static uint32_t _timerClockSet  = 84000000;
const static int32_t _autoReload = 4200 - 1;
const static int _motorPolarity[2] = {1/*Left*/, 1/*Right*/};

static int32_t _motorSpeed[2] = {0/*Left*/, 0/*Right*/};

static MotorMode_t _motorMode = MOTOR_STOP;  
static int _motorUpgradeTag = 0;

/* Timer use - Test use */
static uint32_t _timeTick = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Motor initialization
  * @param  None
  * @retval None
  */
void MotorInit(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable TIM3, GPIOC and GPIOG clocks *********************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
  /* GPIO configuration **************************************************/
  /* GPIO configured as follows:
        - Pin -> PG2, PG3, PG4, PG5  
        - Output Mode
        - GPIO speed = 100MHz
        - Push pull output mode
        - Pull-up
  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  /* GPIO configuration **************************************************/
  /* GPIO configured as follows:
        - Pin -> PC6, PC7  
        - Alternate function (AF) Mode -> TIM3 CH1 & CH2
        - GPIO speed = 100MHz
        - Push pull output mode
        - Pull-up
  */
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       
	GPIO_Init(GPIOC, &GPIO_InitStructure);              
	  
  /* Time Base configuration *********************************************/
  /* Timer3 Base configured as follows:
        - Prescaler (Psc) = 84MHz
        - Count up mode
        - Auto-Reload Register (ARR) value = 4200 -> 20KHz
        - not divide system clock
     ## - RepetitionCounter 
  */
	TIM_TimeBaseStructure.TIM_Prescaler = (_timerClockAPB1 / _timerClockSet) - 1;  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = _autoReload;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Channel 1,2 and 3 Configuration in PWM mode *************************/
  /* TIM Output Compare configured as follows:
        - Use PWM mode 1 : PWM mode 2 is contrary to mode 1
        - Output polarity High level
          ## PWM mode 1 is When CNT < CCRx, output the polarity e-level
             our e-level is High level. 

                       CNT ^
                           |
                       ARR |-----/-----/-----/-----/---
                           |    /!    /!    /!    /
                      CCRx |---/-!---/-!---/-!---/-----
                           |  /! !  /! !  /! !  /!
                           | / ! ! / ! ! / ! ! / !
                           |/  ! !/  ! !/  ! !/  !
                           |----------------------------> t
                           |   ! !   ! !   ! !   !
                         1 |___! !___! !___! !___! 
                           |   | |   | |   | |   |
                           |   | |   | |   | |   |
                         0 |   |_|   |_|   |_|   |_
                  IO Logic |---------------------------->

        - Capture Compare Register (CCR) = 0
        - Output Compare state enable
     ## - Complementary Output Compare state
     ## - Complementary output polarity
     ## - Output Compare pin state
     ## - Complementary Output Compare pin state
  */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 0;
  
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  
  /* Use Preload Registor ************************************************/
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
  UpdateMotorState(MOTOR_STOP);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int SetMotorDutyRatio(double leftDutyRatio, double rightDutyRatio)
{
  if(rightDutyRatio > 1 || leftDutyRatio > 1 ||
     leftDutyRatio < -1 || rightDutyRatio < -1) { return _motorMode; }
  
  return SetMotorPulse((int32_t)(leftDutyRatio * _autoReload),
											 (int32_t)(rightDutyRatio * _autoReload));
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int SetMotorPulse(int32_t leftPulse, int32_t rightPulse)
{
  if(rightPulse >  _autoReload) rightPulse = _autoReload;
  else if (rightPulse <  0) rightPulse = 0;
     
  if(leftPulse >  _autoReload) leftPulse = _autoReload;
  else if (leftPulse <  0) leftPulse = 0;
  
  _motorSpeed[1] = rightPulse;
  _motorSpeed[0] =  leftPulse;
  
  TIM_SetCompare1(TIM3, (uint32_t)(_motorSpeed[1]));
	TIM_SetCompare2(TIM3, (uint32_t)(_motorSpeed[0]));

  return _motorUpgradeTag;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SetMotorState(MotorMode_t motorMode)
{
  if(_motorMode != motorMode)
  {
    _motorMode = motorMode;
    _motorUpgradeTag = 1;
  }  
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
MotorMode_t UpgradeMotorState(void)
{
  if (_motorUpgradeTag != 0)
  {
    _motorUpgradeTag = 0;
    GPIO_ResetBits(GPIOG, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
    SysTickDelay(100);

    switch (_motorMode) {
    case MOTOR_STOP:                         break;
    case MOTOR_FRONT: {
      GPIO_SetBits(GPIOG, GPIO_Pin_5 | GPIO_Pin_2);
    }                                        break;
    case MOTOR_TURN_LEFT: {
      GPIO_SetBits(GPIOG, GPIO_Pin_2 | GPIO_Pin_4);
    }                                        break;
    case MOTOR_TURN_RIGHT: {
      GPIO_SetBits(GPIOG, GPIO_Pin_3 | GPIO_Pin_5);
    }                                        break;
    case MOTOR_BACK: {
      GPIO_SetBits(GPIOG, GPIO_Pin_4 | GPIO_Pin_3);
    }                                        break;
    default:                                 break;
    }
  }
  
  return _motorMode;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
MotorMode_t UpdateMotorState(MotorMode_t motorMode)
{
  if(motorMode != _motorMode ){
    _motorUpgradeTag = 0;
    GPIO_ResetBits(GPIOG, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
    SysTickDelay(100);

    switch (motorMode) {
    case MOTOR_STOP:                         break;
    case MOTOR_FRONT: {
      GPIO_SetBits(GPIOG, GPIO_Pin_5 | GPIO_Pin_2);
    }                                        break;
    case MOTOR_TURN_LEFT: {
      GPIO_SetBits(GPIOG, GPIO_Pin_2 | GPIO_Pin_4);
    }                                        break;
    case MOTOR_TURN_RIGHT: {
      GPIO_SetBits(GPIOG, GPIO_Pin_3 | GPIO_Pin_5);
    }                                        break;
    case MOTOR_BACK: {
      GPIO_SetBits(GPIOG, GPIO_Pin_4 | GPIO_Pin_3);
    }                                        break;
    default:                                 break;
    }
    
    _motorMode = motorMode;
  }
  
  return _motorMode;
}

/**
  * @brief  GetSpeed  '0' is Left
                      '1' is Right
  * @param  None
  * @retval None
  */
int32_t GetMotorSpeed(int8_t leftOrRight)
{
  return _motorSpeed[leftOrRight & 0x01];
}

/* Test functions ------------------------------------------------------------*/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void SetMotorForwardTime(uint32_t sec)
{
  _timeTick = sec;
}

/**
  * @brief  Reference Only -> (F12) SysTick_Handler();
  * @param  None
  * @retval None
  */
void _forwardTime_Interrupt()
{
  ;    if(_timeTick >  1)
  {
    _timeTick--;
  }
  else if(_timeTick == 1)
  {
    _timeTick--;
    if(_motorMode != MOTOR_STOP){
      _motorUpgradeTag = 1;
      _motorMode = MOTOR_STOP;
    }
  }
}
