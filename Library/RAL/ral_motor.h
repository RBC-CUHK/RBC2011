#ifndef __RAL_MOTOR_H
#define __RAL_MOTOR_H

#define SELF		0
#define MODE2103	1

struct Motor_Struct{
int controlA; //99 dummy
int controlB; //99 dummy
int pwmChannel;
int currPWM;
void (*Motor_SetPWM)(struct Motor_Struct* setting,int pwm);
};

struct Motor_Struct* Motor_Init(struct Motor_Struct* MS, int pwmChannel,int controlA, int controlB, char Mode);
void Motor_SetPWM(struct Motor_Struct* MS, int PWM);
void Motor_Set2103PWM(struct Motor_Struct*, int PWM);
void Motor_SetOwnPWM(struct Motor_Struct* MS, int PWM);
#endif
