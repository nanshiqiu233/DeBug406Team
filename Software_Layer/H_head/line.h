/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/line.h 
  * @author  Debug406 Team
  * @date    16-April-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention

    TODO: Serial communication - Busy#
          Serial listening (Listening for PID arguments) - Read arguments;
          Motor control (After commands) - To take the air line;
          Gray sensor (Read ADC arguments for PID control) - To take the air line;
          Gyro - To discern if the car is on uphill;
          Waitting to go to TRANS_TO_END_MODE - End of the mode#

          ## Maybe ##
          Motor's encoder - To calculate the speed of the car;
          Gyro - To get the argument and add it to PID control arithmetic;

  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_LINE_S
#define DEBUGROB_LINE_S

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "motor.h"
#include "serial.h"
#include "buttom.h"
#include "main.h"
#include "line.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void GoOnLine(void);
void GoStraight(void);
int  PointRecognition(void);
int  PitchRecognition(void);
void UphillGoStraight(void);
int32_t GetTransSpeed(void);

void GoOnCircle(void);

int GetZeroPointNumber(int place);
int GetNagtivePointNumber(int place);
uint32_t GetDelayMs(void);
void SetBaseSpeed(int32_t baseSpeed);
void SetTriggerDelay(uint16_t ms);
uint16_t GetTriggerDelay(void);
void ADCUpdate(void);;
	
/* New PID Interface */
void GoOnLine_New(void);
void TestGoStraight(void);

void _ArgumentsUpdate_Callback(uint8_t args[]);
void _PointDelay_Interruput(void);

#endif /* !DEBUGROB_LINE_S */
