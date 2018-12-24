/** See a brief introduction (right-hand button) */
#include "serial.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _BUFFERSIZE    50   /* char */
#define _START_SIG     0xF0
#define _END_SIG       '\n'

#define _USART_SILENT  0x00
#define _USART_RESPON  0x11

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void _BufferIntegrate_Callback(uint8_t upperBuffer[static _BUFFERSIZE]);

/* Private functions ---------------------------------------------------------*/

#ifndef _USE_BLUETOOTH
/**
 ===============================================================================
                  ##### USART1 Serial to Upper Computer #####
 =============================================================================== 
  * @brief  Initialization USART1's hardware.
  * @param  None
  * @retval None
  */
void SerialInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* Enable USART1 and GPIOA clocks *************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  /* GPIO configuration *************************************************/
  /* GPIO configured as follows:
        - Pin -> PA9 & PA10  
        - Alternate function (AF) Mode -> USART1
        - GPIO speed = 50MHz
        - Push pull output mode
        - Pull-up
  */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* USARTx configuration *************************************************/
  /* USART1 configured as follows:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART1 ********************************************************/
  USART_Cmd(USART1, ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* NVIC configuration ***************************************************/
  /* NVIC configured as follows:
        - Interrupt function name = USART1_IRQHandler
        - pre-emption priority = 3 (Very low)
     ** - subpriority level = 3 (Very low)
        - NVIC_IRQChannel enable
  */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SerialSend(char buffer[], int lenth)
{
  for(uint8_t i = 0; i < lenth; i++)
  {
    USART_SendData(USART1, buffer[i]);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
  }
}

/**
  * @brief  USART1_IRQn's interrupt function (F12) USART1_IRQHandler().
  * @param  None
  * @retval None
  */
void _UsartReceive_Interrupt(void)
{
  static uint8_t buffer[_BUFFERSIZE] = { '\0' };
  static int handle = -1;
  static int tag = _USART_SILENT;
  
  /* Protocol configuration *************************************************/
  /* Protocol as follows:
        - Message Frame is setting as:
  
          ---------------------------------------------------
          | STR_SIG ! LENTH ! MES1_H ! MES1_L ! ... ! END_L |
          ---------------------------------------------------
          |  0xF0   ! 0xhh  !  0xhh  !  0xhh  ! ... ! '\n'  |
          ---------------------------------------------------
  
        - If We get StartFrame, We will clean the receive buffer.
        - If We get EndFrame and the buffer lenths is a even number, 
          send it to function _BufferIntegrate() and update message;
   */
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
    uint8_t tmpc = USART_ReceiveData(USART1);
    
    if(tag == _USART_SILENT && tmpc == _START_SIG)
    {
      tag = _USART_RESPON;
      handle = -1;
    }
    else
    {
      buffer[++handle] = tmpc;
      if(handle == buffer[0])
      {
        if(buffer[handle] == _END_SIG)
        {
          _BufferIntegrate_Callback(buffer);
        }
        tag = _USART_SILENT;
      }
    }
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _BufferIntegrate_Callback(uint8_t upperBuffer[])
{  
  int lenth = upperBuffer[0];

  switch(lenth)
  {
  case 2: break;
  default:break;
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#else // define _USE_BLUETOOTH

/**
 ===============================================================================
                     ##### USART3 Serial to Blue Tooth #####
 =============================================================================== 
  * @brief  Initialization USART3's hardware.
  * @param  None
  * @retval None
  */
void SerialInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  /* Enable USART3 and GPIOA clocks *************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  /* GPIO configuration *************************************************/
  /* GPIO configured as follows:
        - Pin -> PB10 & PB11  
        - Alternate function (AF) Mode -> USART3
        - GPIO speed = 50MHz
        - Push pull output mode
        - Pull-up
  */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* USARTx configuration *************************************************/
  /* USART3 configured as follows:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure);

  /* Enable USART3 ********************************************************/
  USART_Cmd(USART3, ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* NVIC configuration ***************************************************/
  /* NVIC configured as follows:
        - Interrupt function name = USART3_IRQHandler
        - pre-emption priority = 3 (Very low)
     ** - subpriority level = 3 (Very low)
        - NVIC_IRQChannel enable
  */
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void SerialSend(char buffer[], int lenth)
{
  for(uint8_t i = 0; i < lenth; i++)
  {
    USART_SendData(USART3, buffer[i]);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
  }
}

/**
  * @brief  USART3_IRQn's interrupt function (F12) USART3_IRQHandler().
  * @param  None
  * @retval None
  */
void _UsartReceive_Interrupt(void)
{
  static uint8_t buffer[_BUFFERSIZE] = { '\0' };
  static int handle = -1;
  static int tag = _USART_SILENT;
  
  /* Protocol configuration *************************************************/
  /* Protocol as follows:
        - Message Frame is setting as:

          ---------------------------------------------------
          | STR_SIG ! LENTH ! MES1_H ! MES1_L ! ... ! END_L |
          ---------------------------------------------------
          |  0xF0   ! 0xhh  !  0xhh  !  0xhh  ! ... ! '\n'  |
          ---------------------------------------------------
  
        - If We get StartFrame, We will clean the receive buffer.
        - If We get EndFrame and the buffer lenths is a even number, 
          send it to function _BufferIntegrate() and update message;
   */
  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
  {
    uint8_t tmpc = USART_ReceiveData(USART3);
    
    if(tag == _USART_SILENT && tmpc == _START_SIG)
    {
      tag = _USART_RESPON;
      handle = -1;
    }
    else
    {
      buffer[++handle] = tmpc;
      if(handle == buffer[0])
      {
        if(buffer[handle] == _END_SIG)
        {
          _BufferIntegrate_Callback(buffer);
        }
        tag = _USART_SILENT;
      }
    }
  }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void _BufferIntegrate_Callback(uint8_t upperBuffer[])
{  
  int lenth = upperBuffer[0];

  switch(lenth)
  {
  case 2: break;
  default:break;
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART3, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#endif //!_USE_BLUETOOTH
