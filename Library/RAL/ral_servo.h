/**
 *	@file
 *	@brief	RAL_Servo Header File
 * */
#ifndef __RAL_SERVO_H
#define	__RAL_SERVO_H
#include "ral_motor.h"

/**
 *	@brief	Store information of a servo motor
 *	
 *	There should be an underlying motor.
 *	Can define the angel range and the PWM value correspoindng to lower and
 *	upper side
 * */
struct Servo_Struct{
	struct Motor_Struct* MS;	///< Underlying Motor
	int PWMLowerSide;			///< PWM value at lower side
	int PWMUpperSide;			///< PWM value at upper side
	int AngleRange;				///< Angel range of the servo motor
	int PWMEach;				///< PWM value for each degree (computed at init)
};

struct Servo_Struct* Servo_Init(struct Servo_Struct* SS, struct Motor_Struct* MS, int PWMLowerSide, int PWMUpperSide, int range);
void Servo_SetRelative(struct Servo_Struct* SS, float angle);
void Servo_SetAbsolute(struct Servo_Struct* SS, float angle);
#endif
