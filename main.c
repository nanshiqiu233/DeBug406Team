/** See a brief introduction (right-hand button) */
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/** @attention avoid someone to use 'machineMode'  
  * @usage MachineMode_t machineMode = END_MODE;
  */
typedef enum machineMode__ 
{
  /* This mode is used in a stage */
  POINT_MODE,
  /* This mode is used in a line */
  LINE_MODE,
  /* This mode is used in LINE_MODE to END_MODE */
  TRANS_TO_END_MODE,
  /* This mode is used in END_MODE to LINE_MODE */
  TRANS_TO_LINE_MODE
} MachineMode_t;
static MachineMode_t _machineMode = POINT_MODE;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifndef _TEST_TAG_

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Local variables *****************************************************/
  
  /* Initialize Step *****************************************************/
  SysTickInit();
  STMMiniBoardInit();
  SerialInit();
  MotorInit();
  AdcInit();
  ServoInit();
  EncoderInit();
  GyroInit();
  
  printf("UpperComputer Reset!");
  SysTickDelay(100);
  
  /* Infinite loop *******************************************************/
  while (TRUE)
  {
    switch (_machineMode) { 
    case POINT_MODE: 
    {
      SendReCall();
      while ( GettingPointCommand() != OVER_ ) {}
        
      _machineMode = TRANS_TO_LINE_MODE;                          
    } 
    
    break; case TRANS_TO_LINE_MODE: 
    {      
      TransitionSpeedReform();      
			
      do 
      {
				ADCUpdate();
        UpdateAngleValue(); 
      } while( !CheckStartPoint() );

      SetBaseSpeed(GetLineSpeed());
      _machineMode = LINE_MODE;
    } 
    
    break; case LINE_MODE: 
    {
      while( !PointRecognition() )
      {
				static int tag = 0;
				UpdateAngleValue();
				ADCUpdate();
				
        if(tag == 0)
				{
					GoOnLine();
				}
        
        /* Long bridge :
           - GYRO value as follow:
              
           ^   Start Signal
           |
           |              -1.8 ~ 1.8 
           |  24   __________________________   -29
           |      /                          \
           |_____/                            \__________
           |--------------------------------------------->

        */
        if( GetRollAngle() < -25 )
        {
          SetMotorPulse(_GOSTRAIGHT_SPEEDLEFT, _GOSTRAIGHT_SPEEDRIGHT);
          tag = 1;
        }		
        else if( GetRollAngle() > 10 && tag == 1 )
        {
          tag = 2;
        }
        else if( GetRollAngle() < 4  && tag == 2 )
        {
          tag = 0;
        }
      }
      
      _machineMode = TRANS_TO_END_MODE;                           
    } 
    
    break; case TRANS_TO_END_MODE: 
    {
      
      /* Point - 6 *CAN NOT USE* */
      if(GetTransSpeed() == 100)
      {
        SetTriggerDelay(400);
        while(!CheckEndPointCheckIn())
        {        
          UpdateAngleValue();
          ADCUpdate();
          
          GoOnCircle();
        }
      }
      /* Point - 7 *CAN NOT USE* */
      else if(GetTransSpeed() == 200)
      {
        SetBaseSpeed(2000);
        while(GetRollAngle() > -6)
        {        
          UpdateAngleValue();
          ADCUpdate();
          
          GoOnLine();
        }
        
        SetBaseSpeed(7000);
        while(GetRollAngle() < -4)
        {        
          UpdateAngleValue();
          ADCUpdate();
          
          GoOnLine();
        }
        
        SysTickDelay(GetDelayMs());
      }
      /* Point - 8 *CAN NOT USE* */
      else if(GetTransSpeed() == 300)
      {
        SetBaseSpeed(5000);
        while(!CheckEndPointCheckIn())
        {        
          UpdateAngleValue();
          ADCUpdate();
          
          GoOnLine();
        }
        
        while(!CheckEndPointCheckIn())
        {        
          UpdateAngleValue();
          ADCUpdate();
          
          GoOnLine();
        }
      }
      /* Normal */
      else
      {
        SetBaseSpeed(GetTransSpeed());
        
        do {
          UpdateAngleValue();
          ADCUpdate();
          
          /* Elimination End-Recognization */
          /* Plane */
          if( GetRollAngle() < 10 && GetRollAngle() > -6 )
          {
            SetBaseSpeed(GetTransSpeed());
            GoOnLine();
          }
          /* Slope */
          else if( GetPitchAngle() < -10)
          {
            SetMotorPulse(1250, 1600);
          }
          else if( GetPitchAngle() > 10)
          {
            SetMotorPulse(1600, 1250);
          }
          /* Roadblock */
          else
          {
            SetBaseSpeed(1300);
            GoOnLine();
          }
        } while( !CheckEndPointCheckIn() );
        
        
        if(GetHighLowPlaceTag())
        {
          SetTriggerDelay(2700);
          SetMotorPulse(_GOSTRAIGHT_SPEEDLEFT, _GOSTRAIGHT_SPEEDRIGHT);
        
          while(GetRollAngle() < -5)
          {
            if(GetTriggerDelay() == 0)
            {
              SetBaseSpeed(3000);
              ADCUpdate();
              GoOnLine();
            }
            UpdateAngleValue();
          }
          SysTickDelay(200);
        }
        else
        {
          do
          {
            ADCUpdate();
            GoOnLine();
          } while(GetZeroPointNumber(1) < 2);
          
          SysTickDelay(GetDelayMs());
        }
        
        SetBaseSpeed(10000);
      }
      UpdateMotorState(MOTOR_STOP);
      _machineMode = POINT_MODE;                                  
    } 
    
    break; default: {}
    }
  }
}

#else

/**
  * @brief  Test main program
  * @param  None
  * @retval None
  */
int /*Test*/ main(void)
{
  /* Local variables */
  
  /* Initialize Step *****************************************************/
  SysTickInit();
  STMMiniBoardInit();
  SerialInit();
  MotorInit();
  AdcInit();
  ServoInit();
  EncoderInit();
  GyroInit();
  
  SysTickDelay(10000);
  //SetMotorPulse(_GOSTRAIGHT_SPEEDLEFT, _GOSTRAIGHT_SPEEDRIGHT);
  
  /* Infinite loop */
  while (TRUE)
  {
    UpdateAngleValue();
    UpdateButtom();
    
    //UpdateMotorState(MOTOR_FRONT);
    //SetServoPWM(SERVO_ARM_RIGHT, 2400);
    printf("%d, %d\r\n", GetRFSpeed(1), GetLRSpeed(1));
    printf("%f, %f, %f\r\n", GetRollAngle(), GetPitchAngle(), GetYawAngle());
    SysTickDelay(1700);
    //ResetServoPWM(SERVO_ARM_RIGHT);
    //SysTickDelay(1300);
    //SysTickDelay(5000);
    //SetMotorPulse(0, 0);
  }
}

#endif //!_TEST_TAG_

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
