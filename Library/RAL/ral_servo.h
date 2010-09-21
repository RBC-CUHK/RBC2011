#ifndef __RAL_SERVO_H
#define	__RAL_SERVO_H
#include "ral_motor.h"

struct Servo_Struct{
	struct Motor_Struct* MS;
	int PWMLowerSide;
	int PWMUpperSide;
	int AngleRange;
	int PWMEach;
};

struct Servo_Struct* Servo_Init(struct Servo_Struct* SS, struct Motor_Struct* MS, int PWMLowerSide, int PWMUpperSide, int range);
void Servo_SetRelative(struct Servo_Struct* SS, int angle);
void Servo_SetAbsolute(struct Servo_Struct* SS, int angle);
#endif
