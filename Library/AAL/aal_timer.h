/**
 *	@file
 *	@brief	AAL_Timer Header File
 * */
#ifndef __AAL_TIMER_H
#define __AAL_TIMER_H

void Timer_Init(int channel,int frequency, void (*callback)(void) __irq);

#endif
