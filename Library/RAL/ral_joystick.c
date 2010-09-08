#include "ral_joystick.h"

#include "../AAL/aal_gpio.h"

#include "../AAL/aal_adc.h"

void Joystick_Init(){
	int button[10] = {L1,L2,R1,R2,B1,B2,B3,B4,BL,BR};
	int analog[4] = {RY,RX,LY,LX};
	int channel;
	int i;
	for ( i = 0; i < 10; i ++){
		//port = button[i] / 100;
		//channel = button[i] % 100;
		GPIO_Init(button[i],INPUT);
	}
	for ( i = 0; i < 4; i++){
		channel = analog[i] % 10;
		ADC_InitChannel(channel);
	}
	return ;
}

int Joystick_ReadAxis(int channel){
	return ADC_Read(channel);
} 

int Joystick_ReadButton(int channel){
	return GPIO_Read(channel);
}
