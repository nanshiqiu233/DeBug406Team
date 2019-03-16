/** See a brief introduction (right-hand button) */
#include "gyro.h"
/* Private include -----------------------------------------------------------*/
#include "stm32f4xx.h"
#include "systick.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t _Usart2_RxBuffer[11] = {0x00};

/* Private functions ---------------------------------------------------------	*/
void Gyro_Init(void)
{
	GPIO_InitTypeDef GPIO_USART2_Init;
	USART_InitTypeDef USART_USART2_Init;
	NVIC_InitTypeDef USART2_DMA_NVIC_Init;
	DMA_InitTypeDef USART2_Receive_DMA_Init;
	
	//时钟开启
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	//PD5 TX口配置
	GPIO_USART2_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_USART2_Init.GPIO_OType = GPIO_OType_PP;
	GPIO_USART2_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_USART2_Init.GPIO_Pin   = GPIO_Pin_5;
	GPIO_USART2_Init.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD,&GPIO_USART2_Init);
	
	//PD6 RX口配置
	GPIO_USART2_Init.GPIO_Pin   = GPIO_Pin_6;
	GPIO_USART2_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_USART2_Init.GPIO_OType = GPIO_OType_OD;
	GPIO_USART2_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_USART2_Init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_USART2_Init);
	
	//引脚复用处理
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
	//USART2初始化
	USART_USART2_Init.USART_BaudRate            = 115200;
	USART_USART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_USART2_Init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_USART2_Init.USART_Parity              = USART_Parity_No;
	USART_USART2_Init.USART_StopBits            = USART_StopBits_1;
	USART_USART2_Init.USART_WordLength          = USART_WordLength_8b;
	
	USART_Init(USART2,&USART_USART2_Init);
	USART_Cmd(USART2,ENABLE);
	
	//DMA配置
	DMA_DeInit(DMA1_Stream5);
	
	USART2_Receive_DMA_Init.DMA_Channel            = DMA_Channel_4;
	USART2_Receive_DMA_Init.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));
	USART2_Receive_DMA_Init.DMA_Memory0BaseAddr    = (uint32_t)_Usart2_RxBuffer;
	USART2_Receive_DMA_Init.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	USART2_Receive_DMA_Init.DMA_BufferSize         = 11;
	USART2_Receive_DMA_Init.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	USART2_Receive_DMA_Init.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	USART2_Receive_DMA_Init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	USART2_Receive_DMA_Init.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	USART2_Receive_DMA_Init.DMA_Mode               = DMA_Mode_Circular;
	USART2_Receive_DMA_Init.DMA_Priority           = DMA_Priority_Medium;
	USART2_Receive_DMA_Init.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	USART2_Receive_DMA_Init.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
	USART2_Receive_DMA_Init.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	USART2_Receive_DMA_Init.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	
	//初始化DMA
	DMA_Init(DMA1_Stream5,&USART2_Receive_DMA_Init);
	
	//开启USART2 DMA接收
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	
	//使能DMA中断
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
	
	//配置NVIC优先级
	USART2_DMA_NVIC_Init.NVIC_IRQChannel                   = DMA1_Stream5_IRQn;
	USART2_DMA_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 3;
	USART2_DMA_NVIC_Init.NVIC_IRQChannelSubPriority        = 2;
	USART2_DMA_NVIC_Init.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&USART2_DMA_NVIC_Init);
	
	//使能DMA
	DMA_Cmd(DMA1_Stream5,ENABLE);
	
}


void Gyro_Usart_Rx_Interrupt(void)
{
	DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);
}


float Gyro_GetRollAngle(void)
{
	uint32_t temp = 0;
	float output = 0;
	
	temp = _Usart2_RxBuffer[3];
	temp <<= 8;
	temp |= _Usart2_RxBuffer[2];
	
	output = (float)temp / (float)32768 * (float)180;
	
	return output;
}

float Gyro_GetPitchAngle(void)
{
	uint32_t temp = 0;
	float output = 0;
	
	temp = _Usart2_RxBuffer[5];
	temp <<= 8;
	temp |= _Usart2_RxBuffer[4];
	
	output = (float)temp / (float)32768 * (float)180;
	
	return output;
}

float Gyro_GetYawAngle(void)
{
	uint32_t temp = 0;
	float output = 0;
	
	temp = _Usart2_RxBuffer[7];
	temp <<= 8;
	temp |= _Usart2_RxBuffer[6];
	
	output = (float)temp / (float)32768 * (float)180;
	
	return output;
}


