/** See a brief introduction (right-hand button) */
#include "main.h"

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
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
  
  SysTickDelay(1000);
  SetMotorDutyRatio(0.5, 0.2);

  /* Infinite loop *******************************************************/
  while (TRUE)
  {
    printf("Hello STM32!\n");
    SysTickDelay(1000);
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

  /* Infinite loop */
  while (TRUE)
  {

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
