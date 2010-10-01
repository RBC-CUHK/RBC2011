#ifndef	__FOURWHEEL_H
#define	__FOURWHEEL_H
#include "RAL/ral_servo.h"
#define MAXSPEED 1000
#define	OFFSET1	-1200
#define	OFFSET2	-700
#define	OFFSET3	300
#define	OFFSET4	300
#define	DELAY	0
void Fourwheel_Init(struct Servo_Struct* SS[4], struct Motor_Struct* MS[4]);
void Fourwheel_SetSpeed(int speed);
void Fourwheel_Forward(void);
void Fourwheel_Backward(void);
void Fourwheel_Leftward(void);
void Fourwheel_Rightward(void);
void Fourwheel_RotateLeft(void);
void Fourwheel_RotateRight(void);
void Fourwheel_Stop(void); 
#endif
