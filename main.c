/** See a brief introduction (right-hand button) */
#include "main.h"

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
  
  SysTickDelay(1000);
  printf(" Hello World!");
  
  SetMotorDutyRatio(0.5, 0.5);
  SetMotorForwardTime(5000);

  /* Infinite loop *******************************************************/
  while (TRUE)
  {
    UpgradeMotorState();
  }
}

#else
/* Private function prototypes -----------------------------------------------*/
void MotorTest(void);

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
  EncoderInit();
  MotorInit();
  
  SysTickDelay(1000);
  printf(" Hello World!\r\n");
  MotorTest();
  
  /* Infinite loop */
  while (TRUE)
  {

  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void MotorTest(void)
{
  uint32_t timerCount[2];
  double duty = 0.00;
  
  SetMotorDutyRatio(0, 0);
  UpdateMotorState(MOTOR_FRONT);
  printf("L, R,\r\n");
  for(; duty < 0.8; duty += 0.04)
  {
    SetMotorDutyRatio(duty, duty);
    SysTickDelay(1000);
    timerCount[0] = GetLRSpeed(2);
    timerCount[1] = GetRFSpeed(2);
    printf("%f,%f,\r\n", 1.0*timerCount[0]/500000, 1.0*timerCount[1]/500000);
  }
  
  for(; duty > 0; duty -= 0.04)
  {
    SetMotorDutyRatio(duty, duty);
    SysTickDelay(1000);
    timerCount[0] = GetLRSpeed(2);
    timerCount[1] = GetRFSpeed(2);
    printf("%f,%f,\r\n", 1.0*timerCount[0]/500000, 1.0*timerCount[1]/500000);
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
