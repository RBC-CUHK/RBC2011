#ifndef __RAL_MOTOR_H
#define __RAL_MOTOR_H
struct Motor_Setting{
int controlA; //99 dummy
int controlB; //99 dummy
int pwmChannel;
int currPWM;
void (*Motor_SetPWM)(struct Motor_Setting* setting,int pwm);
};

struct Motor_Setting Motor_Init(int pwmChannel, void (*Motor_SetPWM)(struct Motor_Setting,int pwm));
void Motor_Set2103PWM(struct Motor_Setting*, int PWM);
void Motor_Set213xPWM(struct Motor_Setting*, int PWM);
#endif
