/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/line.h 
  * @author  Debug406 Team
  * @date    1-March-2019
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
	*	This file is used to describe the functions called when 
	* hunting and how the related algorithms work.
  ******************************************************************************
  */
	#ifndef DEBUGROB_LOGIC_H
	#define DEBUGROB_LOGIC_H
	/*Hunting with laser tube and gray sensor return values*/
	/*
		This algorithm looks for two eigenvalues of the function fitted by the array
		of grayscale sensors returned via the ADC, namely the small value median and 
		the small value width.
	******************************************************************************
	*/
	/*Find the closest number in the data and return its PWM duty cycle*/
	int _FindRightSpeed(int NowSpeed);
	/*Store PWM duty cycle versus speed()*/
	int _SpeedToDutyCycle(void);
	/*Use the core part of the grayscale sensor and laser tube algorithm. If you need to modify it, please refer to the README.MD file under the LOGIC file.*/
	int _TrackingCoreAlgorithm(int NowSpeed,int PerfectSpeed);
	#endif
