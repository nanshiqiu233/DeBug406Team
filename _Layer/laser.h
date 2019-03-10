/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/laser.h 
  * @author  Debug406 Team
  * @date    2-January-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
    # Laser(Digital) 高反射率表面  LED亮 OUT输出低电平
                     低反射率表面  LED灭 OUT输出高电平

		# 例如           在线上        高反射率 LED亮 低电平
		                 在绿毯上      低反射率 LED灭 高电平
										 
		# 因此           低反射率到高反射率      负跳变 下降沿
										 高反射率到低反射        正跳变 上升沿
										 
		# 引脚设置       左边激光管        PG13 	输入模式 无拉电阻
										 右边激光管		     PG11		输入模式 无拉电阻
		
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_LASER_H
#define DEBUGROB_LASER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum laserState__
{
	
  LOW = 0,
  HIGH= 1
	
} LaserState_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions (F12)-------------------------------------------------- */
void Laser_Init(void);
LaserState_t GetLaserState(void);

/* Reference Only -> (F12) EXTI0_IRQHandler(); */
/*static*/ inline void _LaserEdgeTrigger_Interrupt(void);

#endif /* !DEBUGROB_LASER_H */
