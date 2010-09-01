#include "ral_motor.h"
#include "../AAL/aal_gpio.h"
#include "../AAL/aal_213xpwm.h"

struct Motor_Setting Motor_Init(int pwmChannel,int controlA, int controlB, char Mode){
	struct Motor_Setting MS;
	MS.controlA = controlA;
	MS.controlB = controlB;
	MS.pwmChannel = pwmChannel;
	MS.currPWM = 0;
	if(Mode == 213xMODE){
		PWM_InitChannel(pwmChannel,0);
		MS.Motor_SetPWM = Motor_Set213xPWM;
	} else {
		MS.Motor_SetPWM = Motor_Set2103PWM;
	}
	return MS;
}

void Motor_Set2103PWM(struct Motor_Setting* MS, int PWM);
void Motor_Set213xPWM(struct Motor_Setting* MS, int PWM){
	PWM_Set(MS->pwmChannel,PWM);
	MS->currPWM = pwm;
	return ;
}
