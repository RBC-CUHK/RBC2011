#include "ral_servo.h"
#include "ral_motor.h"
struct Servo_Struct* Servo_Init(struct Servo_Struct* SS, struct Motor_Setting* MS, int PWMLowerSide, int PWMUpperSide, int range){
	SS->MS = MS;
	SS->PWMLowerSide = PWMLowerSide;
	SS->PWMUpperSide = PWMUpperSide;
	SS->AngleRange = range;
	SS->PWMEach = (PWMUpperSide - PWMLowerSide) / range;
	return SS;
}

void Servo_SetRelative(struct Servo_Struct* SS, int angle){
	int currPWM = SS->MS->currPWM;
	int newPWM = currPWM + angle * SS->PWMEach;
	if(newPWM > SS->PWMUpperSide)newPWM = SS->PWMUpperSide;
	else if(newPWM < SS->PWMLowerSide)newPWM = SS->PWMLowerSide;
	SS->MS->Motor_SetPWM(SS->MS,newPWM);
	return;
}

void Servo_SetAbsolute(struct Servo_Struct* SS, int angle){
	int newPWM = angle * SS->PWMEach + SS->PWMLowerSide;
	if(newPWM > SS->PWMUpperSide)newPWM = SS->PWMUpperSide;
	else if(newPWM < SS->PWMLowerSide)newPWM = SS->PWMLowerSide;
	SS->MS->Motor_SetPWM(SS->MS,newPWM);
	return;
}
