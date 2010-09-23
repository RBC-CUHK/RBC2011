#include "armversion.h"
#include "aal_timer.h"

void Timer_Init(int channel,int frequency, void (*callback)(void) __irq){
//	frequency = 10;//using 10hz for debug
	
	if (channel==0){
	   T0PR = 0;									// set prescaler to 0
	   T0MR0 = 13824000/frequency;					// set interrupt rate
	   												// Pclk/1KHz = (11059200 x 5)/(4 x 1000)
       T0MCR = 3;                             		// Interrupt and Reset on MR0
       T0TCR = 1;                             		// Timer0 Enable
	   VICVectAddr0 = (unsigned long)callback; 		// set interrupt vector in 0
       VICVectCntl0 = 0x20 | 4;                    	// use it for Timer 0 Interrupt
       VICIntEnable = 0x00000010;                  	// Enable Timer0 Interrupt
    }
	if (channel==1){
	   T1PR = 0;									// set prescaler to 0
	   T1MR0 = 13824000/frequency;					// set interrupt rate
	   											    // Pclk/1KHz = (11059200 x 5)/(4 x 1000)
       T1MCR = 3;                             		// Interrupt and Reset on MR0
       T1TCR = 1;                             		// Timer1 Enable
	   VICVectAddr1 = (unsigned long)callback;		// set interrupt vector in 1
       VICVectCntl1 = 0x20 | 4;                    	// use it for Timer 1 Interrupt
       VICIntEnable = 0x00000020;                  	// Enable Timer1 Interrupt
    }   
}
