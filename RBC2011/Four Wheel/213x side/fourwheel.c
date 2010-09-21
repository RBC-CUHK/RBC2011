#include "fourwheel.h"
#include "RAL/ral_servo.h"

struct Servo_Struct* FL;
struct Servo_Struct* FR;
struct Servo_Struct* BL;
struct Servo_Struct* BR;

void Fourwheel_Init(struct Servo_Struct* SS[4]){
	FL = SS[0];
	FR = SS[1];
	BR = SS[2];
	BL = SS[3];
	return ;
}

void Fourwheel_Forward(void){
	Servo_SetAbsolute(FL,0);
	Servo_SetAbsolute(FR,90);
	Servo_SetAbsolute(BR,0);
	Servo_SetAbsolute(BL,90);
	return ;
}

void Fourwheel_Backward(void){
	Servo_SetAbsolute(FL,0);
	Servo_SetAbsolute(FR,90);
	Servo_SetAbsolute(BR,0);
	Servo_SetAbsolute(BL,90);
	return ;
}

void Fourwheel_Leftward(void){
	Servo_SetAbsolute(FL,90);
	Servo_SetAbsolute(FR,0);
	Servo_SetAbsolute(BR,90);
	Servo_SetAbsolute(BL,0);
	return ;
}

void Fourwheel_Rightward(void){
	Servo_SetAbsolute(FL,90);
	Servo_SetAbsolute(FR,0);
	Servo_SetAbsolute(BR,90);
	Servo_SetAbsolute(BL,0);
	return ;
}

void Fourwheel_RotateLeft(void){
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);
	return ;
}

void Fourwheel_RotateRight(void){
	Servo_SetAbsolute(FL,45);
	Servo_SetAbsolute(FR,45);
	Servo_SetAbsolute(BR,45);
	Servo_SetAbsolute(BL,45);
	return ;
}
