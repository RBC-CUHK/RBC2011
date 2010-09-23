#ifndef	__FOURWHEEL_H
#define	__FOURWHEEL_H
#include "RAL/ral_servo.h"
void Fourwheel_Init(struct Servo_Struct* SS[4], struct Motor_Struct* MS[4]);
void Fourwheel_Forward(void);
void Fourwheel_Backward(void);
void Fourwheel_Leftward(void);
void Fourwheel_Rightward(void);
void Fourwheel_RotateLeft(void);
void Fourwheel_RotateRight(void); 
#endif