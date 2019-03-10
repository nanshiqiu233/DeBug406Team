#include "line.h"

float previous_error = 0;

double _GetADCError(void)
{
  double Error,Bias;
  double _TURE=0.000082755733553;
  const AdcData_t * adcData;
  uint16_t temp1=0,temp2=0;
  adcData = UpdateButtom();
  for(int i=0; i<=3; i++)
    temp1+=adcData->array[0][i];
  for(int i=4; i<=7; i++)
    temp2+=adcData->array[0][i];
  Bias=(sqrt(temp1*1.0)-sqrt(temp2*1.0))/(temp1+temp2);
  //printf("%f\r\n",Bias);
  Error=_TURE-Bias;
  return Error;
}
double _TrackingCoreAlgorithm(void)
{
  float Kp = 51.763, Ki = 0, Kd = 9.6;
	
  float error = 0, P = 0.0, I = 0., D = 0, PID_value = 0;
  error=_GetADCError();
  //printf("%f\r\n",error);
  P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  //printf("%f\r\n",PID_value);
  //SetMotorDutyRatio(0.15+PID_value, 0.15+PID_value);
  previous_error = error;
  return PID_value;
}
void _GoLine(void)
{
  //printf("%f,%f\r\n",_LastSpeed,_LastSpeedL);
  SetMotorDutyRatio(0.14+_TrackingCoreAlgorithm(), 0.14-_TrackingCoreAlgorithm());
  //SetWheelSpeed((float)0.50+_TrackingCoreAlgorithm(),(float)0.50-_TrackingCoreAlgorithm());
}
