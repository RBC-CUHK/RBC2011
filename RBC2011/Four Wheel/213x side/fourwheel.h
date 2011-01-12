#ifndef	__FOURWHEEL_H
#define	__FOURWHEEL_H
#include "RAL/ral_servo.h"

//These offsets are for the first prototype
#define	OFFSET1 100
#define	OFFSET2	-900
#define	OFFSET3	-1000
#define	OFFSET4	-200

#define	DELAY	0

typedef enum {FORWARD,BACKWARD,LEFTWARD,RIGHTWARD,ROTATELEFT,ROTATERIGHT,STOP,FREE} Fourwheel_Status;
void Fourwheel_Init(struct Servo_Struct* SS[4], struct Motor_Struct* MS[4]);
void Fourwheel_SetSpeed(float speed);
void Fourwheel_Forward(void);
void Fourwheel_Backward(void);
void Fourwheel_Leftward(void);
void Fourwheel_Rightward(void);
void Fourwheel_RotateLeft(void);
void Fourwheel_RotateRight(void);
void Fourwheel_Stop(void); 
#endif
