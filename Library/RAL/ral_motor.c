/**
 *	@file
 *	@brief	RAL_Motor Function Implementation
 * */
#include "armversion.h"
#include "ral_motor.h"
#include "ral_mux.h"
#include "../AAL/aal_gpio.h"
#include "../AAL/aal_pwm.h"
#include "../AAL/aal_spi.h"

/**
 *	@brief	MBMux is the Mux used to communicate with LPC2103.
 *
 *	Should exist in main.c or somewhere else even there would not be any
 *	communication with LPC2103
 * */
extern struct Mux_Struct MBMux;

/**
 *	@brief	Init a Motor Struct.
 *
 *	Set up necessary paramters like PWM Channel, Control Bits 
 *	and anchor the appropriate function pointer of Set PWM 
 *	according to Mode selected
 *
 *	@see	Motor_Struct
 *	@param	MS			The Motor Struct Pointer
 *	@param	pwmChannel	PWM Channel
 *	@param	controlA	Control Bit A
 *	@param	controlB	Control Bit B
 *	@param	Mode		SELF or MODE2103
 *
 *	@return	Pointer to the inited Motor_Struct 
 * */
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

/**
 *	@brief	Set PWM of a motor
 *
 *	PWM would be set through the corresponding Function anchored
 *	
 *	@see	Motor_SetOwnPWM()
 *	@see	Motor_Set2103PWM()
 *	@param	MS		Motor to be set
 *	@param	PWM		PWM Value
 * */
void Motor_SetPWM(struct Motor_Struct* MS, int PWM){
	MS->Motor_SetPWM(MS,PWM);
	return ;
}

/**
 *	@brief	Set PWM value on LPC2103
 *
 *	This function is used to send Set PWM command to LPC2103 through SPI
 *
 * 	@see	Motor_Init()
 * 	@see	Motor_SetPWM()
 * 	@see	Motor_SetOwnPWM()
 *	@param	MS		Motor to be set
 *	@param	PWM		PWM Value
 *
 * */
void Motor_Set2103PWM(struct Motor_Struct* MS, int PWM){
	int data;
	int negative = 0;
	if(PWM < 0){
		PWM *= -1;
		negative = 1;
		}
	if(PWM > 1000)
		PWM = 1000;
	
	if(negative == 1)
		PWM |= 0x0800;
	data = ((MS->pwmChannel & 0x0F)<<12) | (PWM & 0x0FFF);
	SPI_SetLength(16);
	Mux_Set(&MBMux,MUX_2103);
	SPI_Send(data);
	Mux_Unset(&MBMux);
	if(negative == 1)
		MS->currPWM = PWM * (-1);
	else
		MS->currPWM = PWM;
	return ;
}
/**
 *	@brief	Set PWM value at current chip
 *
 *	@param	MS		Motor to be set
 *	@param	PWM		PWM Value
 *	*/
void Motor_SetOwnPWM(struct Motor_Struct* MS, int PWM){
	MS->currPWM = PWM;
	if(PWM < 0){
		PWM *= -1;
		GPIO_Set(MS->controlA,0);
		GPIO_Set(MS->controlB,1);	
	} else {
		GPIO_Set(MS->controlA,1);
		GPIO_Set(MS->controlB,0);
	}
	PWM_Set(MS->pwmChannel,PWM);
	
	return ;
}
