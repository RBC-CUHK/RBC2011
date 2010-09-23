#include "ral_motor.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_pwm.h"
#include "AAL/aal_spi.h"
#include "ral_mux.h"

extern struct Mux_Struct MBMux;

struct Motor_Struct* Motor_Init(struct Motor_Struct* MS,int pwmChannel,int controlA, int controlB, char Mode){
	MS->controlA = controlA;
	MS->controlB = controlB;
	MS->pwmChannel = pwmChannel;
	MS->currPWM = 0;
	if(Mode == SELF){
		PWM_InitChannel(pwmChannel,0);		
		GPIO_Init(controlA,OUTPUT);
		GPIO_Init(controlB,OUTPUT);
		MS->Motor_SetPWM = Motor_SetOwnPWM;
	} else {
		MS->Motor_SetPWM = Motor_Set2103PWM;
	}

	return MS;
}

void Motor_SetPWM(struct Motor_Struct* MS, int PWM){
	MS->Motor_SetPWM(MS,PWM);
	return ;
}

void Motor_Set2103PWM(struct Motor_Struct* MS, int PWM){
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
void Motor_SetOwnPWM(struct Motor_Struct* MS, int PWM){
	if(PWM < 0){
		PWM *= -1;
		GPIO_Set(MS->controlA,0);
		GPIO_Set(MS->controlB,1);	
	} else {
		GPIO_Set(MS->controlA,1);
		GPIO_Set(MS->controlB,0);
	}
	PWM_Set(MS->pwmChannel,PWM);
	MS->currPWM = PWM;
	return ;
}
