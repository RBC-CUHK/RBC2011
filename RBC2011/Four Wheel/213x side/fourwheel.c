#include "fourwheel.h"
#include "RAL/ral_servo.h"

struct Servo_Struct* FL;
struct Servo_Struct* FR;
struct Servo_Struct* BR;
struct Servo_Struct* BL;

struct Motor_Struct* FLM;
struct Motor_Struct* FRM;
struct Motor_Struct* BRM;
struct Motor_Struct* BLM;

int Fourwheel_Speed = 500;

void Fourwheel_Init(struct Servo_Struct* SS[4], struct Motor_Struct* MS[4]){
	FL = SS[0];
	FR = SS[1];
	BR = SS[2];
	BL = SS[3];
	
	FLM = MS[0];
	FRM = MS[1];
	BRM = MS[2];
	BLM = MS[3];

	return ;
}

void Fourwheel_Forward(void){
//	Fourwheel_Stop();
	Servo_SetAbsolute(FL,0);
	Servo_SetAbsolute(FR,90);
	Servo_SetAbsolute(BR,0);
	Servo_SetAbsolute(BL,90);

	Motor_SetPWM(FLM,-Fourwheel_Speed);
	Motor_SetPWM(FRM,Fourwheel_Speed);
	Motor_SetPWM(BRM,Fourwheel_Speed);
	Motor_SetPWM(BLM,-Fourwheel_Speed);
	return ;
}

void Fourwheel_Backward(void){
//	Fourwheel_Stop();
	Servo_SetAbsolute(FL,0);
	Servo_SetAbsolute(FR,90);
	Servo_SetAbsolute(BR,0);
	Servo_SetAbsolute(BL,90);

	Motor_SetPWM(FLM,Fourwheel_Speed);
	Motor_SetPWM(FRM,-Fourwheel_Speed);
	Motor_SetPWM(BRM,-Fourwheel_Speed);
	Motor_SetPWM(BLM,Fourwheel_Speed);
	return ;
}

void Fourwheel_Leftward(void){
//	Fourwheel_Stop();
	Servo_SetAbsolute(FL,90);
	Servo_SetAbsolute(FR,0);
	Servo_SetAbsolute(BR,90);
	Servo_SetAbsolute(BL,0);

	Motor_SetPWM(FLM,Fourwheel_Speed);
	Motor_SetPWM(FRM,Fourwheel_Speed);
	Motor_SetPWM(BRM,-Fourwheel_Speed);
	Motor_SetPWM(BLM,-Fourwheel_Speed);
	return ;
}

void Fourwheel_Rightward(void){
//	Fourwheel_Stop();
	Servo_SetAbsolute(FL,90);
	Servo_SetAbsolute(FR,0);
	Servo_SetAbsolute(BR,90);
	Servo_SetAbsolute(BL,0);

	Motor_SetPWM(FLM,-Fourwheel_Speed);
	Motor_SetPWM(FRM,-Fourwheel_Speed);
	Motor_SetPWM(BRM,Fourwheel_Speed);
	Motor_SetPWM(BLM,Fourwheel_Speed);
	return ;
}

void Fourwheel_RotateLeft(void){
//	Fourwheel_Stop();
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);
	
	Motor_SetPWM(FLM,Fourwheel_Speed);
	Motor_SetPWM(FRM,Fourwheel_Speed);
	Motor_SetPWM(BRM,Fourwheel_Speed);
	Motor_SetPWM(BLM,Fourwheel_Speed);
	return ;
}

void Fourwheel_RotateRight(void){
//	Fourwheel_Stop();
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);

	Motor_SetPWM(FLM,-Fourwheel_Speed);
	Motor_SetPWM(FRM,-Fourwheel_Speed);
	Motor_SetPWM(BRM,-Fourwheel_Speed);
	Motor_SetPWM(BLM,-Fourwheel_Speed);
	return ;
}

void Fourwheel_Stop(void){
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);

	Motor_SetPWM(FLM,0);
	Motor_SetPWM(FRM,0);
	Motor_SetPWM(BRM,0);
	Motor_SetPWM(BLM,0);
	return ;
}
