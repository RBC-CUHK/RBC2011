/**
 *	@file
 *	@brief	RAL_Sevo Function Implementation
 * */
#include "ral_servo.h"
#include "ral_motor.h"

/**
 *	@brief	Init the servo
 *
 *	PWM value for each degree is calculated
 *
 *	@param	SS				Servo_Struct to be inited
 *	@param	MS				Underlying Motor_Struct
 *	@param	PWMLowerSide	PWM at lower side
 *	@param	PWMUpperSide	PWM at upper side
 *	@param	range			Angel Range
 *	
 *	@return	Pointer to inited Servo_Struct
 * */
struct Servo_Struct* Servo_Init(struct Servo_Struct* SS, struct Motor_Struct* MS, int PWMLowerSide, int PWMUpperSide, int range){
	SS->MS = MS;
	SS->PWMLowerSide = PWMLowerSide;
	SS->PWMUpperSide = PWMUpperSide;
	SS->AngleRange = range;
	SS->PWMEach = (PWMUpperSide - PWMLowerSide) / range;
	return SS;
}

/**
 *	@brief	Set the servo motor to relative angle
 *
 *	Make use of the CurrPWM stored at the Motor_Struct
 *
 *	@param	SS		Servo_Struct to be set
 *	@param	angle	The angle
 * */
void Servo_SetRelative(struct Servo_Struct* SS, float angle){
	int currPWM = SS->MS->currPWM;
	int newPWM = currPWM + angle * (float)SS->PWMEach;
	if(newPWM > SS->PWMUpperSide)newPWM = SS->PWMUpperSide;
	else if(newPWM < SS->PWMLowerSide)newPWM = SS->PWMLowerSide;
	Motor_SetPWM(SS->MS,newPWM);
	return;
}

/**
 *	@brief	Set the servo motor to absolute angle
 *
 *	@param	SS		Servo_Struct to be set
 *	@param	angle	The angle
 * */
void Servo_SetAbsolute(struct Servo_Struct* SS, float angle){
	int newPWM = angle * (float)SS->PWMEach + SS->PWMLowerSide;
	if(newPWM > SS->PWMUpperSide)newPWM = SS->PWMUpperSide;
	else if(newPWM < SS->PWMLowerSide)newPWM = SS->PWMLowerSide;
	Motor_SetPWM(SS->MS,newPWM);
	return;
}
