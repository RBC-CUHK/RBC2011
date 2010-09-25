/**
 *	@file
 *	@brief	RAL_Motor Header File
 * */
#ifndef __RAL_MOTOR_H
#define __RAL_MOTOR_H

/// PWM is generated in the current chip
#define SELF		0
/// PWM is generated in the LPC2103
#define MODE2103	1

/**
 *	@brief	Store information of a motor
 *
 *	Two Control Bits A and B
 *	When A = 1, B = 0, PWM > 0, Motor should rotate clockwise
 * */
struct Motor_Struct{
	int controlA;	///< GPIO for the controlA bit
	int controlB;	///< GPIO for the controlB bit
	int pwmChannel;	///< PWMChannel
	int currPWM;	///< Store the current PWM value
	///Function Anchor Point for Set PWM
	void (*Motor_SetPWM)(struct Motor_Struct* setting,int pwm);
};

struct Motor_Struct* Motor_Init(struct Motor_Struct* MS, int pwmChannel,int controlA, int controlB, char Mode);
void Motor_SetPWM(struct Motor_Struct* MS, int PWM);
void Motor_Set2103PWM(struct Motor_Struct*, int PWM);
void Motor_SetOwnPWM(struct Motor_Struct* MS, int PWM);
#endif
