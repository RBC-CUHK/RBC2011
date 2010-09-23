#ifndef __AAL_213XPWM_H
#define __AAL_213XPWM_H

/*
#define POWER_0 1400
#define POWER_1 1400
#define POWER_2 1400
#define POWER_3 1400
#define POWER_4 1400
#define POWER_5 1400
#define POWER_6 1400
*/

void PWM_InitPeriod(int period);		//Init the PWM Period
void PWM_InitFrequency(int frequency);
void PWM_InitChannel(int channel, int target_level);
void PWM_Set(int channel, int level);
void PWM_SetPercentage(int channel,int percentage);

#endif
