#include "armversion.h"
#ifdef	__LPC213x_H
#include "aal_213xpwm.h"

void PWM_InitFrequency(int frequency){	
	PWMMR0 = 13824000 / frequency;
	return ;
}

void PWM_InitPeriod(int period){		//Init the PWM Period
	PWMMR0 = period;
	return ;
}

void PWM_InitChannel(int channel,int level){
	int i;
	int n;

	switch(channel){				
	   case 1: PINSEL0 |= 1<< 1; break;	// set p0.0 to PWM1
	   case 2: PINSEL0 |= 1<<15; break;	// set p0.7 to PWM2
	   case 3: PINSEL0 |= 1<< 3; break;	// set p0.1 to PWM3
	   case 4: PINSEL0 |= 1<<17; break;	// set p0.8 to PWM4
	   case 5: PINSEL1 |= 1<<10; break; // set p0.21 to PWM5
	   case 6: PINSEL0 |= 1<<19; break;	// set p0.9 to PWM6	   	   	   
    } 
	
	PWMPCR |= 1<<(8+channel);	//The PWM output enable
	PWMMCR |= (1<<1);			//PWM match control reg: reset timer counter if MR0 matches
//	PWMMR0 = 1/PWM_PERIOD;		//match registers for pwm, MR0~6
	
//	if (level>1000)
//	   level=1000;
	switch(channel){			//set motor PWM width 				
	   case 1: PWMMR1 = level; break;
	   case 2: PWMMR2 = level; break;
	   case 3: PWMMR3 = level; break;
	   case 4: PWMMR4 = level; break;
	   case 5: PWMMR5 = level; break;
	   case 6: PWMMR6 = level; break;
    } 

	PWMLER |= 1<<channel;		//latch enable register
	PWMTCR |= (1<<0) | (1<<3);	//counter enable, PWM enable
	
//	PWM_Set(channel, 0);
	
	for(n = 0 ; n < 100 ; n++){		//safety buffer
		for(i=0 ; i< 450; i++){}
	}
}




void PWM_Set(int channel,int level){	//power select(0~1000)
	
//    if( level > 1000){
//		level = 1000;
	
	switch(channel){
		case 1: PWMMR1 = level; break;
		case 2: PWMMR2 = level; break;
		case 3: PWMMR3 = level; break;
		case 4: PWMMR4 = level; break;
		case 5: PWMMR5 = level; break;
		case 6: PWMMR6 = level; break;
/*
	   case 1: PWMMR1 = level * POWER_1 / 1000.0;
	   		   break;
	   case 2: PWMMR2 = level * POWER_2 / 1000.0;
	   	   	   break;
	   case 3: PWMMR3 = level * POWER_3 / 1000.0;
	   	   	   break;
	   case 4: PWMMR4 = level * POWER_4 / 1000.0;
	   	   	   break;
	   case 5: PWMMR5 = level * POWER_5 / 1000.0;
	   	   	   break;
	   case 6: PWMMR6 = level * POWER_6 / 1000.0;
	   	   	   break;
*/	
    } 
	PWMLER |= (1<<channel);
}

void PWM_SetPercentage(int channel,int percentage){
	switch(channel){
		case 1: PWMMR1 = PWMMR0 * percentage / 100; break;
		case 2: PWMMR2 = PWMMR0 * percentage / 100; break;
		case 3: PWMMR3 = PWMMR0 * percentage / 100; break;
		case 4: PWMMR4 = PWMMR0 * percentage / 100; break;
		case 5: PWMMR5 = PWMMR0 * percentage / 100; break;
		case 6: PWMMR6 = PWMMR0 * percentage / 100; break;
    } 
	PWMLER |= (1<<channel);
}
#endif
