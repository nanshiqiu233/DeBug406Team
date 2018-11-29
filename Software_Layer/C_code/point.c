/** See a brief introduction (right-hand button) */
#include "point.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RESET   0
#define SET     1

#define TURN_RIGHT_TRIGGER 178
#define TURN_LEFT__TRIGGER 178
#define TURN_BACK__TRIGGER 370

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char _messageToUpperComputer[] = {0xFF, 0x03, 0x01, 0x00, 0xAA};
static const int _bufferLenth = 5;
static int _commandUpdateTag = RESET;
static PointCommand_t _command = POINT_DEFAULT;
static uint8_t _arguments[2] = { 0, 0 };
static uint8_t _lineModeSpeed = 10;

/* Private function prototypes -----------------------------------------------*/
void _CommandUpdate_Callback(PointCommand_t newCom, uint8_t args[]);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void HandsUp(void)
{
  SetServoPWM(SERVO_ARM_RIGHT, 2300);
  SetServoPWM(SERVO_ARM_LEFT, 2300);
  
  SysTickDelay(500);
  
  ResetServoPWM(SERVO_ARM_RIGHT);
  ResetServoPWM(SERVO_ARM_LEFT);
  SendReCall();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void HeadMove(void)
{
  SetServoPWM(SERVO_HEAD, 2300);
  
  SysTickDelay(500);
  
  ResetServoPWM(SERVO_HEAD);
  SendReCall();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TurnRegAngleLeft(void)
{
  SetMotorPulse(_GOSTRAIGHT_SPEEDLEFT, _GOSTRAIGHT_SPEEDRIGHT);
  UpdateMotorState(MOTOR_TURN_LEFT);

  WaittingTrigger(TURN_LEFT__TRIGGER);
  
  UpdateMotorState(MOTOR_STOP);
  SendReCall();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TurnRegAngleRight(void)
{
  SetMotorPulse(3000, 3000);
  UpdateMotorState(MOTOR_TURN_RIGHT);
  // 178
  WaittingTrigger(TURN_RIGHT_TRIGGER);
  
  UpdateMotorState(MOTOR_STOP);
  SendReCall();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TurnBack(void)
{
  SetMotorPulse(3000, 3000);
  UpdateMotorState(MOTOR_TURN_LEFT);
  //360 -> 380 -> 370
  WaittingTrigger(TURN_BACK__TRIGGER);
  
  UpdateMotorState(MOTOR_STOP);
  SendReCall();
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void TurnAngle(int16_t plusAngle)
{
  if( plusAngle > 180 || plusAngle < -180 ) return;
  int trigger = 0;
  
  if( plusAngle > 0) 
  {
    trigger = (int) (_CONSTANT_OF_ROTATE_BASE * plusAngle);
    SetMotorState(MOTOR_TURN_LEFT);
  }
  else 
  {
    trigger = (int) (_CONSTANT_OF_ROTATE_BASE * (-plusAngle));
    SetMotorState(MOTOR_TURN_RIGHT);
  }
  
  SetMotorPulse(3000, 3000);
  
  WaittingTriggerWithFunc(trigger, (void (*)(void))UpgradeMotorState);
  UpdateMotorState(MOTOR_STOP);
  SendReCall();
}

void SendReCall(void)
{
  SerialSend(_messageToUpperComputer, _bufferLenth);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int GettingPointCommand(void)
{
  if(_commandUpdateTag != RESET)
  {
    _commandUpdateTag = RESET;  
      
    switch(_command){
    /* Servo *********************************************/
    case HAND_UP :   HandsUp();           break;
    case HEAD_MOVE : HeadMove();          break;
    /* Motor *********************************************/
    case TURN_RIGHT: TurnRegAngleRight(); break;
    case TURN_LEFT : TurnRegAngleLeft();  break;
    case TURN_BACK : TurnBack();          break;
    case TURN_ANYANGLE : {
      TurnAngle(((int16_t)_arguments[0] << 8) | _arguments[1]);
    }                                     break;
    /* Quit **********************************************/
    case OVER_:
    case QUIT_:                           break;

    default:{}break;
    }
    _command = POINT_DEFAULT;
  }    
  
  return _command;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _CommandUpdate_Callback(PointCommand_t newCom, uint8_t args[])
{
  _command = newCom;
  _arguments[0] = args[0];
  _arguments[1] = args[1];
  _lineModeSpeed = args[2];
  _commandUpdateTag = SET;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t GetLineSpeed(void)
{
  if(_lineModeSpeed > 0 && _lineModeSpeed < 20)
    return _lineModeSpeed * 1000;
  return 0;
}

