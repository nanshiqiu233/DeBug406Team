#include "line.h"
//		/**
//  * @brief  Store an array for speed and PWM duty cycle conversion
//  * @param  None
//  * @retval None
//  */
//	double _SpeedToDutyCycleL(void)
//	{
//	double _SpeedToDuty [21][2]={{0.079343, 0.0},
//															 {0.3937925, 0.01},
//															 {0.8916565, 0.02},
//															 {1.40568, 0.03},
//															 {1.9030945, 0.04},
//															 {2.3306505, 0.05},
//															 {2.6573745, 0.06},
//															 {2.906644, 0.07},
//															 {3.089984, 0.08},
//															 {3.2416315, 0.09},
//															 {3.377761, 0.1},
//															 {3.492357, 0.11},
//															 {3.5862925, 0.12},
//															 {3.663863, 0.13},
//															 {3.7226135, 0.14},
//															 {3.778989, 0.15},
//															 {3.827382, 0.16},
//															 {3.865302, 0.17},
//															 {3.901428, 0.18},
//															 {3.9333675,0.19},
//															 {3.963261, 0.2}
//															 };		
//	}
	/**
  * @brief  To choose a right speed and feed back
  * @param  Now speed 
  * @retval None
  */
	double _FindRightSpeedLR(int NowSpeed)
	{
		NowSpeed=GetLRSpeed(1);
		NowSpeed=GetLRSpeed(1);
	if(NowSpeed<=0.079343)
		return 0.0;
	else if(NowSpeed<=0.3937925)
		return 0.01;
	else if(NowSpeed<=0.8916565)
		return 0.02;
	else if(NowSpeed<=1.40568)
		return 0.03;
	else if(NowSpeed<=1.9030945)
		return 0.04;
	else if(NowSpeed<=2.3306505)
		return 0.05;
	else if(NowSpeed<=2.6573745)
		return 0.06;
	else if(NowSpeed<=2.906644)
		return 0.07;
	else if(NowSpeed<=3.089984)
		return 0.08;
	else if(NowSpeed<=3.2416315)
		return 0.09;
	else if(NowSpeed<=3.377761)
		return 0.10;
	else if(NowSpeed<=3.492357)
		return 0.11;
	else if(NowSpeed<=3.5862925)
		return 0.12;
	else if(NowSpeed<=3.663863)
		return 0.13;
	else if(NowSpeed<=3.7226135)
		return 0.14;
	else if(NowSpeed<=3.778989)
		return 0.15;
	else if(NowSpeed<=3.827382)
		return 0.16;
	else if(NowSpeed<=3.865302)
		return 0.17;
	else if(NowSpeed<=3.901428)
		return 0.18;
	else if(NowSpeed<=3.9333675)
		return 0.19;
	else if(NowSpeed<=3.963261)
		return 0.20;
	else 
		return 0.99;
	}
  void _GetADCError(void)
	{
		const AdcData_t *adc_adrr;
		adc_adrr = GetADCData();
	}
	void _TrackingCoreAlgorithm(void)
{
	float Kp = 10, Ki = 0.5, Kd = 0;                    
	float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
	float previous_error = 0, previous_I = 0;
	  for (int i = 0; i < 2; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
     
    }
  }
	P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) - (Kd * D);
  SetMotorDutyRatio(0.15+PID_value, 0.15+PID_value);
  previous_error = error;
}