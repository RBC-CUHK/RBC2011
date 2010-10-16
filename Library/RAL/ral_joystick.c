/**
 *	@file
 *	@brief RAL_Joystick Function Implementation
 * */
#include "ral_joystick.h"

#include "../AAL/aal_gpio.h"
#include "../AAL/aal_adc.h"

/**
 *	Init the Joystick settings according to configuration at Header file
 * */
void Joystick_Init(void){
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

/**
 *	Read out the Joystick Axis Reading
 *
 *	@param	channel	Josytick Axis	(LX/LY/RX/RY)
 *	@return	ADC reading of the Axis
 * */
int Joystick_ReadAxis(int channel){
	return ADC_Read(channel);
} 

/**
 *	Check whether the Joystick Button is pressed
 *
 *	@param	channel	Joystick Button	(B1/B2/B3/B4/BL/BR)
 *	@return	0:not pressed	1:pressed
 * */
int Joystick_ReadButton(int channel){
	return (GPIO_Read(channel) + 1) % 2;
}
