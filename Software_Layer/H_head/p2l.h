/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/p2l.h 
  * @author  Debug406 Team
  * @date    17-Fabrulary-2018
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  
    TODO: Serial communication - Not busy#
          Serial listening (Listening for PID arguments) - Read arguments;
          Motor control (After commands) - To take the air line;
          Gray sensor (Read ADC arguments) - To confirm  the car is on the line.
          Go to LINE_MODE - End of the mode#
          
    TODO: Serial communication - Not busy#
          Motor control (After commands) - To take the air line & Slow down;
          Gray sensor (Read ADC arguments) - To discern the 'Point';
          Motor control (After identification a point) - stop & turn the angles we set;
          Go to END_MODE - End of the mode#
  
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUGROB_POINTTOLINE_S
#define DEBUGROB_POINTTOLINE_S

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "motor.h"
#include "gyro.h"
#include "serial.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TransitionSpeedReform(void);
int  CheckStartPoint(void);
int  CheckEndPointCheckIn(void);
//int  CheckEndPointCheckOut(void);
void GoStraightCorrection_TransitionMode(void);
int  GetHighLowPlaceTag(void);

#endif /* DEBUGROB_POINTTOLINE_S */
