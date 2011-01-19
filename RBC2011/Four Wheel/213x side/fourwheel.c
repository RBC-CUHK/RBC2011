#include "fourwheel.h"
#include "RAL/ral_servo.h"

#define Fourwheel_SetStatus(s) do {if(Status == (s)) return ; else Status = (s);} while(0)
struct Servo_Struct* FL;
struct Servo_Struct* FR;
struct Servo_Struct* BR;
struct Servo_Struct* BL;

struct Motor_Struct* FLM;
struct Motor_Struct* FRM;
struct Motor_Struct* BRM;
struct Motor_Struct* BLM;

static int __MaxSpeed = 1000;
signed char __MotorDirection[4] = {0};
Fourwheel_Status Status = STOP;

void Fourwheel_Init(struct Servo_Struct* SS[4], struct Motor_Struct* MS[4]){
	FL = SS[0];
	FR = SS[1];
	BR = SS[2];
	BL = SS[3];
	
	FLM = MS[0];
	FRM = MS[1];
	BRM = MS[2];
	BLM = MS[3];

	Fourwheel_Stop();

	return ;
}

void Fourwheel_SetSpeed(float _speed){
	int speed = 0;
	if(_speed > 1)
		_speed = 1;
	else if(_speed < -1)
		_speed = -1;
	speed = _speed * __MaxSpeed;	
	Motor_SetPWM(FLM,speed*__MotorDirection[0]);
	Motor_SetPWM(FRM,speed*__MotorDirection[1]);
	Motor_SetPWM(BRM,speed*__MotorDirection[2]);
	Motor_SetPWM(BLM,speed*__MotorDirection[3]);	
}

void Fourwheel_SetMaxSpeed(int MaxSpeed){
	__MaxSpeed = MaxSpeed;
}

void Fourwheel_Forward(void){
	Fourwheel_SetStatus(FORWARD);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,0);
	Servo_SetAbsolute(FR,90);
	Servo_SetAbsolute(BR,0);
	Servo_SetAbsolute(BL,90);
	__MotorDirection[0] = -1;
	__MotorDirection[1] = 1;
	__MotorDirection[2] = 1;
	__MotorDirection[3] = -1;
	return ;
}

void Fourwheel_Backward(void){
	Fourwheel_SetStatus(BACKWARD);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,0);
	Servo_SetAbsolute(FR,90);
	Servo_SetAbsolute(BR,0);
	Servo_SetAbsolute(BL,90);
	__MotorDirection[0] = 1;
	__MotorDirection[1] = -1;
	__MotorDirection[2] = -1;
	__MotorDirection[3] = 1;
	return ;
}

void Fourwheel_Leftward(void){
	Fourwheel_SetStatus(LEFTWARD);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,90);
	Servo_SetAbsolute(FR,0);
	Servo_SetAbsolute(BR,90);
	Servo_SetAbsolute(BL,0);
	__MotorDirection[0] = 1;
	__MotorDirection[1] = 1;
	__MotorDirection[2] = -1;
	__MotorDirection[3] = -1;
	return ;
}

void Fourwheel_Rightward(void){
	Fourwheel_SetStatus(RIGHTWARD);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,90);
	Servo_SetAbsolute(FR,0);
	Servo_SetAbsolute(BR,90);
	Servo_SetAbsolute(BL,0);
	__MotorDirection[0] = -1;
	__MotorDirection[1] = -1;
	__MotorDirection[2] = 1;
	__MotorDirection[3] = 1;
	return ;
}

void Fourwheel_RotateLeft(void){
	Fourwheel_SetStatus(ROTATELEFT);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);
	__MotorDirection[0] = 1;
	__MotorDirection[1] = 1;
	__MotorDirection[2] = 1;
	__MotorDirection[3] = 1;
	return ;
}

void Fourwheel_RotateRight(void){
	Fourwheel_SetStatus(ROTATERIGHT);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);
	__MotorDirection[0] = -1;
	__MotorDirection[1] = -1;
	__MotorDirection[2] = -1;
	__MotorDirection[3] = -1;
	return ;
}

void Fourwheel_Stop(void){
	Fourwheel_SetStatus(STOP);
	Fourwheel_SetSpeed(0);
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);
	__MotorDirection[0] = 0;
	__MotorDirection[1] = 0;
	__MotorDirection[2] = 0;
	__MotorDirection[3] = 0;
	return ;
}
