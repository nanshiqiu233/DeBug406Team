/** See a brief introduction (right-hand button) */
#include "serial.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t _Command_RxBuffer[10] = {0x00};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Serial_Init(void)
{
	GPIO_InitTypeDef GPIO_USART1_Init;
	GPIO_InitTypeDef GPIO_USART3_Init;
	
	USART_InitTypeDef USART_USART1_Init;
	USART_InitTypeDef USART_USART3_Init;
	
	NVIC_InitTypeDef USART1_DMA_NVIC_Init;
	NVIC_InitTypeDef USART3_DMA_NVIC_Init;
	
	DMA_InitTypeDef USART1_Receive_DMA_Init;
	DMA_InitTypeDef USART3_Receive_DMA_Init;
	
	
	//时钟开启
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	/*
		USART1初始化以及USART1 DMA接收配置开始
	*/	
	
	//PA9 TX口配置
	GPIO_USART1_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_USART1_Init.GPIO_OType = GPIO_OType_PP;
	GPIO_USART1_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_USART1_Init.GPIO_Pin   = GPIO_Pin_9;
	GPIO_USART1_Init.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA,&GPIO_USART1_Init);
	
	//PA10 RX口配置
	GPIO_USART1_Init.GPIO_Pin   = GPIO_Pin_10;
	GPIO_USART1_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_USART1_Init.GPIO_OType = GPIO_OType_OD;
	GPIO_USART1_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_USART1_Init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_USART1_Init);
	
	//引脚复用处理
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//USART1初始化
	USART_USART1_Init.USART_BaudRate            = 115200;
	USART_USART1_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_USART1_Init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_USART1_Init.USART_Parity              = USART_Parity_No;
	USART_USART1_Init.USART_StopBits            = USART_StopBits_1;
	USART_USART1_Init.USART_WordLength          = USART_WordLength_8b;
	
	USART_Init(USART1,&USART_USART1_Init);
	USART_Cmd(USART1,ENABLE);
	
	//DMA配置
	DMA_DeInit(DMA2_Stream5);
	
	USART1_Receive_DMA_Init.DMA_Channel            = DMA_Channel_4;
	USART1_Receive_DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
	USART1_Receive_DMA_Init.DMA_Memory0BaseAddr    = (uint32_t)_Command_RxBuffer;
	USART1_Receive_DMA_Init.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	USART1_Receive_DMA_Init.DMA_BufferSize         = 10;
	USART1_Receive_DMA_Init.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	USART1_Receive_DMA_Init.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	USART1_Receive_DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART1_Receive_DMA_Init.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	USART1_Receive_DMA_Init.DMA_Mode               = DMA_Mode_Circular;
	USART1_Receive_DMA_Init.DMA_Priority           = DMA_Priority_Medium;
	USART1_Receive_DMA_Init.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	USART1_Receive_DMA_Init.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
	USART1_Receive_DMA_Init.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	USART1_Receive_DMA_Init.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	
	//初始化DMA
	DMA_Init(DMA2_Stream5,&USART1_Receive_DMA_Init);
	
	//开启USART1 DMA接收
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	//使能DMA中断
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);

	//配置NVIC优先级
	USART1_DMA_NVIC_Init.NVIC_IRQChannel                   = DMA2_Stream5_IRQn;
	USART1_DMA_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 3;
	USART1_DMA_NVIC_Init.NVIC_IRQChannelSubPriority        = 2;
	USART1_DMA_NVIC_Init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&USART1_DMA_NVIC_Init);
	
	//使能DMA
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
	
	/*
		USART1初始化以及USART1 DMA接收配置结束
	*/	


	/*
		USART3初始化以及USART3 DMA接收配置开始
	*/

	//PB10 TX口配置
	GPIO_USART3_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_USART3_Init.GPIO_OType = GPIO_OType_PP;
	GPIO_USART3_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_USART3_Init.GPIO_Pin   = GPIO_Pin_10;
	GPIO_USART3_Init.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB,&GPIO_USART3_Init);
	
	//PB11 RX口配置
	GPIO_USART3_Init.GPIO_Pin   = GPIO_Pin_11;
	GPIO_USART3_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_USART3_Init.GPIO_OType = GPIO_OType_OD;
	GPIO_USART3_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_USART3_Init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_USART3_Init);

	//引脚复用处理
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//USART3初始化
	USART_USART3_Init.USART_BaudRate            = 115200;
	USART_USART3_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_USART3_Init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_USART3_Init.USART_Parity              = USART_Parity_No;
	USART_USART3_Init.USART_StopBits            = USART_StopBits_1;
	USART_USART3_Init.USART_WordLength          = USART_WordLength_8b;
	
	USART_Init(USART3,&USART_USART3_Init);
	USART_Cmd(USART3,ENABLE);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//DMA配置
	DMA_DeInit(DMA1_Stream1);
	
	USART3_Receive_DMA_Init.DMA_Channel            = DMA_Channel_4;
	USART3_Receive_DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));
	USART3_Receive_DMA_Init.DMA_Memory0BaseAddr    = (uint32_t)_Command_RxBuffer;
	USART3_Receive_DMA_Init.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	USART3_Receive_DMA_Init.DMA_BufferSize         = 10;
	USART3_Receive_DMA_Init.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	USART3_Receive_DMA_Init.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	USART3_Receive_DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART3_Receive_DMA_Init.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	USART3_Receive_DMA_Init.DMA_Mode               = DMA_Mode_Circular;
	USART3_Receive_DMA_Init.DMA_Priority           = DMA_Priority_Medium;
	USART3_Receive_DMA_Init.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	USART3_Receive_DMA_Init.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
	USART3_Receive_DMA_Init.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	USART3_Receive_DMA_Init.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	
	//初始化DMA
	DMA_Init(DMA1_Stream1,&USART3_Receive_DMA_Init);
	
	//开启USART3 DMA接收
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	
	//使能DMA中断
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);

	//配置NVIC优先级
	USART3_DMA_NVIC_Init.NVIC_IRQChannel                   = DMA1_Stream1_IRQn;
	USART3_DMA_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 3;
	USART3_DMA_NVIC_Init.NVIC_IRQChannelSubPriority        = 2;
	USART3_DMA_NVIC_Init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&USART3_DMA_NVIC_Init);
	
	//使能DMA
	DMA_Cmd(DMA1_Stream1,ENABLE);
	
	/*
		USART3初始化以及USART3 DMA接收配置结束
	*/


}


PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	
	
	//以后会有专门的数据发送函数 printf效率低是众所周知的
	//除非调试用途 请不要再用printf了
	
	//现在的printf会同时作用于上位机与蓝牙串口
	
	USART_SendData(USART1, (uint8_t) ch);
  USART_SendData(USART3, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


