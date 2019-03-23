#ifndef SKILL_H
#define SKILL_H
#include "cpu_test.h"
#include "motor.h"
#include "servo.h"
#include "encoder.h"
#include "buttom.h"
#include "gyro.h"
#include "laser.h"
#include "line.h"
#include "point.h"
#include "UpHillOrDownHill.h"
#include "motor_turn.h"
void InitFlag(void);
void ThreeTOFour(void);
void OneTOTwo(void);
void TwoTOThree(void);
extern int FlagUp;
extern int FlagDown;
extern int _FlagBack;
extern int FlagPoint;
extern int TurnFlag;
#endif
