#include "ral_motor.h"
#include "../AAL/aal_gpio.h"
#include "../AAL/aal_213xpwm.h"
#include "../AAL/aal_spi.h"
#include "ral_mux.h"

extern struct Mux_Struct MBMux;

struct Motor_Setting Motor_Init(int pwmChannel,int controlA, int controlB, char Mode){
	struct Motor_Setting MS;
	MS.controlA = controlA;
	MS.controlB = controlB;
	MS.pwmChannel = pwmChannel;
	MS.currPWM = 0;
	if(Mode == MODE213x){
		PWM_InitChannel(pwmChannel,0);
		MS.Motor_SetPWM = Motor_Set213xPWM;
	} else {
		MS.Motor_SetPWM = Motor_Set2103PWM;
	}
	return MS;
}

void Motor_Set2103PWM(struct Motor_Setting* MS, int PWM){
	int data;
	int negative = 0;
	SPI_setLength(16);
	if(PWM < 0){
		PWM *= -1;
		negative = 1;
		}
	if(PWM > 1000)
		PWM = 1000;
	
	if(negative == 1)
		PWM |= 0x0800;
	data = ((MS->pwmChannel & 0x0F)<<12) | (PWM & 0x0FFF);
	Mux_Set(&MBMux,MUX_2103);
	SPI_Send(data);
	Mux_Unset(&MBMux);
	if(negative == 1)
		MS->currPWM = PWM * (-1);
	else
		MS->currPWM = PWM;
	return ;
}
void Motor_Set213xPWM(struct Motor_Setting* MS, int PWM){
	PWM_Set(MS->pwmChannel,PWM);
	MS->currPWM = PWM;
	return ;
}
