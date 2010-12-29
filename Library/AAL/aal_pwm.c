/**
 *	@file
 *	@brief	AAL_PWM	Function Implementation
 * */
#include "armversion.h"
#include "aal_pwm.h"

#ifdef	DOXYGEN_ONLY
#define	__LPC213x_H
#define __LPC2103_H
#endif

/// Store the PWM period
int globalperiod = 0;

/**
 *	@brief	Init the Frequency of the PWM Channels
 *	@param	frequency	Frequency in Hz
 *	@see	PWM_InitPeriod
 * */
void PWM_InitFrequency(int frequency){	
	PWM_InitPeriod(13824000 / frequency);
	return ;
}

/**
 *	@brief	Set T On Percentage
 *
 *	@param	channel		PWM channel to be set
 *	@param	percentage	T On percentage
 *
 *	@see	PWM_Set
 * */
void PWM_SetPercentage(int channel,int percentage){
	PWM_Set(channel,globalperiod * percentage / 100);
	return ;
}

#ifdef	__LPC213x_H
/*
 *	@brief	Init the period of the PWM Channels
 *	@param	period	PWM period
 * */
void PWM_InitPeriod213x(int period){		//Init the PWM Period
	PWMMR0 = period;
	globalperiod = period;
   	PWMMCR |= (1<<1);			//PWM match control reg: reset timer counter if MR0 matches
	PWMTCR |= (1<<0) | (1<<3);	//counter enable, PWM enable
	return ;
}

/**
 *	@brief	Init the PWM channel
 *
 *	Perform pin select and PWM output enable.
 *	Set the PWM value specified
 *
 *	@param	channel	PWM channel to be inited
 *	@param	level	PWM value
 *
 *	@see	PWM_Set213x
 * */
void PWM_InitChannel213x(int channel,int level){

	switch(channel){				
		case 1: PINSEL0 |= 1<< 1; break;	// set p0.0 to PWM1
		case 2: PINSEL0 |= 1<<15; break;	// set p0.7 to PWM2
		case 3: PINSEL0 |= 1<< 3; break;	// set p0.1 to PWM3
		case 4: PINSEL0 |= 1<<17; break;	// set p0.8 to PWM4
		case 5: PINSEL1 |= 1<<10; break; // set p0.21 to PWM5
		case 6: PINSEL0 |= 1<<19; break;	// set p0.9 to PWM6	   	   	   
	} 

	PWMPCR |= 1<<(8+channel);	//The PWM output enable

	PWM_Set213x(channel,level);

	return ;
}

/**
 *	@brief	Set PWM value
 *	
 *	@param	channel	PWM channel to be set
 *	@param	level	PWM value
 * */
void PWM_Set213x(int channel,int level){
	if(level > globalperiod)
		level = globalperiod;	

	switch(channel){
		case 1: PWMMR1 = level; break;
		case 2: PWMMR2 = level; break;
		case 3: PWMMR3 = level; break;
		case 4: PWMMR4 = level; break;
		case 5: PWMMR5 = level; break;
		case 6: PWMMR6 = level; break;
		default: return;
	} 
	PWMLER |= (1<<channel);
	return ;
}

#endif
#ifdef	__LPC2103_H
/*
 *
 *	 LPC 2103
 *
 * */

void _setPWM(int timer, int channel, int value);

/**
 *	@brief	Init the PWM channels to output
 * */
void PWM_InitOutput(void){
	/** Match Output Enable **/
	T0EMR=(0x01 <<0)|(0x01 <<2);
	T1EMR=(0x01 <<0)|(0x01 <<1)|(0x01 <<2);
	T2EMR=(0x01 <<1)|(0x01 <<2);
	T3EMR=(0x01 <<0)|(0x01 <<3);
	/****/

	/** Match Control **/
	T0EMR|=(0x01 <<4)|(0x01 <<8);
	T1EMR|=(0x01 <<4)|(0x01 <<6)|(0x01 <<8);
	T2EMR|=(0x01 <<6)|(0x01 <<8);
	T3EMR|=(0x01 <<4)|(0x01 <<10);

	T0MCR|=(1 <<10);
	T1MCR|=(1 <<10);
	T2MCR|=(1 <<10);
	T3MCR|=(1 <<4);

	/** PWM Enable **/
	T0PWMCON|=(0x01 <<0)|(0x01 <<2);
	T1PWMCON|=(0x01 <<0)|(0x01 <<1)|(0x01 <<2);
	T2PWMCON|=(0x01 <<1)|(0x01 <<2);
	T3PWMCON|=(0x01 <<0)|(0x01 <<3);
	/****/

	/** Timer Enable **/
	T0TCR = 1; T1TCR = 1; T2TCR = 1; T3TCR = 1;
	/****/
	return ;
}

/**
 *	@brief	Init the period of the PWM Channels
 *
 *	Update all the four timers period.
 *	@param	period	PWM period
 *	@see	PWM_InitOutput
 * */
void PWM_InitPeriod2103(int period){		//Init the PWM Period
	T0MR3 = period;
	T1MR3 = period;
	T2MR3 = period;
	T3MR1 = period;
	globalperiod = period;
	PWM_InitOutput();
	return ;
}


/**
 *	@brief	Init the PWM channel
 *
 *	Perform pin select and PWM output enable.
 *	Set the PWM value specified
 *
 *	@param	channel	PWM channel to be inited
 *	@param	level	PWM value
 *	@see	PWM_Set2103
 * */
void PWM_InitChannel2103(int channel,int level){
//	int i;
//	int n;
	switch(channel){				
		case 0: PINSEL0 |= (0x02 <<6);
		case 2: PINSEL1 |= (0x02 <<0);   	   	   
		case 10:PINSEL0 |= (0x02 <<24);
		case 11:PINSEL0 |= (0x02 <<26);
		case 12:PINSEL1 |= (0x02 <<6);
		case 21:PINSEL0 |= (0x02 <<16);
		case 22:PINSEL0 |= (0x02 <<18);
		case 30:PINSEL1 |= (0x02 <<10);
		case 33:PINSEL1 |= (0x02 <<28);
	} 
	PWM_Set(channel,level);	
	//	for(n = 0 ; n < 100 ; n++){		//safety buffer
	//		for(i=0 ; i< 450; i++){}
return ;
}

/**
 *	@brief	Set PWM value
 *
 *	@param	channel	PWM channel to be set
 *	@param	level	PWM value
 *	@see	_setPWM
 * */
void PWM_Set2103(int channel,int level){
	_setPWM(channel/10,channel%10,level);
	return ;
}

/**
 *	@brief	Underlying set PWM function
 *
 *	@param	timer	PWM timer
 *	@param	channel	PWM channel
 *	@param	value	PWM Value
 * */
void _setPWM(int timer, int channel, int value){
	if(value > globalperiod)
		value = 0;
	else if (value == globalperiod) 
		value = globalperiod + 100;
	else
		value = globalperiod - value;
	
	switch (timer){
		case 0:
			switch (channel){
				case 0: T0MR0 = value; break;
				case 1: T0MR1 = value; break;
				case 2: T0MR2 = value; break;
				case 3: T0MR3 = value; break;
			}
			break;

		case 1:
			switch (channel){
				case 0: T1MR0 = value; break;
				case 1: T1MR1 = value; break;
				case 2: T1MR2 = value; break;
				case 3: T1MR3 = value; break;
			}
			break;
		case 2:
			switch (channel){
				case 0: T2MR0 = value; break;
				case 1: T2MR1 = value; break;
				case 2: T2MR2 = value; break;
				case 3: T2MR3 = value; break;
			}
			break;
		case 3:
			switch (channel){
				case 0: T3MR0 = value; break;
				case 1: T3MR1 = value; break;
				case 2: T3MR2 = value; break;
				case 3: T3MR3 = value; break;
			}
			break;
	}
}
#endif

#ifdef	DOXYGEN_ONLY
#undef	__LPC213x_H
#undef	__LPC2103_H
#undef	__ARMVERSION_H
#include "armversion.h"
#endif
