/** See a brief introduction (right-hand button) */
#include "laser.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static LaserState_t _LLaserState = HIGH;
static LaserState_t _RLaserState = HIGH;

static LaserChangeState_t LLaserChangeState = UnChange;
static LaserChangeState_t RLaserChangeState = UnChange;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init laser's pin & interrupt.
  * @param  None
  * @retval None
  */
void Laser_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable External port clocks *****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
  /* GPIO configuration *************************************************/
  /* GPIO configured as follows:
        - Pin -> PG11 PG13  
        - Input Mode
        - No-Pull
  */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  
  /* EXTI configuration *************************************************/
  /* EXTI configured as follows:
        - Source -> PG13 PG15
        - Interrupt Mode
        - Rising & Falling Interrupt
        - Enable
  */  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource13);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_Init(&EXTI_InitStructure);
	
  /* NVIC configuration ***************************************************/
  /* NVIC configured as follows:
        - Interrupt function name = USART1_IRQHandler
        - pre-emption priority = 0 (Very low)
     ** - subpriority level = 2 (Very low)
        - NVIC_IRQChannel enable
  */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

/**
  * @brief  EXTI_IRQn's interrupt function (F12) EXTI15_10_IRQHandler().
  * @param  None
  * @retval None
  */
void _LaserEdgeTrigger_Interrupt(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) == SET)
	{
		LLaserChangeState = Changed;
		
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13) == SET)
		{
			_LLaserState = HIGH;
		}
		else
		{
			_LLaserState = LOW;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line13);


	if(EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		RLaserChangeState = Changed;
		
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11) == SET)
		{
			_RLaserState = HIGH;
		}
		else
		{
			_RLaserState = LOW;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line11);
	
}

/**
  * @brief  Get Left Laser's state.
  * @param  None
  * @retval Laser State. Can be High or Low.
  */
LaserState_t GetLeftLaserState(void)
{
  return _LLaserState;
}


/**
  * @brief  Get Right Laser's state.
  * @param  None
  * @retval Laser State. Can be High or Low.
  */
LaserState_t GetRightLaserState(void)
{
  return _RLaserState;
}

/**
  * @brief  See if the Left Laser State Changed.
  * @param  None
  * @retval Laser Change State. Can be Changed or Unchange.
  */
LaserChangeState_t IsLLaserChange(void)
{
	return LLaserChangeState;
}

/**
  * @brief  See if the Right Laser State Changed.
  * @param  None
  * @retval Laser Change State. Can be Changed or Unchange.
  */
LaserChangeState_t IsRLaserChange(void)
{
	return RLaserChangeState;
}

/**
  * @brief  Clear the Pending Bit of LLaser Change State to Unchange.
  * @param  None
  * @retval None
  */
void ClearLLaserChangePendingBit(void)
{
	LLaserChangeState = UnChange;
}


/**
  * @brief  Clear the Pending Bit of RLaser Change State to Unchange..
  * @param  None
  * @retval None
  */
void ClearRLaserChangePendingBit(void)
{
	RLaserChangeState = UnChange;
}

