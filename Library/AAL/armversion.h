#include <lpc213x.h>
//#include <lpc2103.h>

#ifdef __LPC2103_H

/* 
 * Register Mapping for GPIO
 *
 * */
#define IO0DIR	IODIR
#define IO0SET	IOSET
#define IO0CLR	IOCLR
#define IO0PIN	IOPIN

/* 
 * Dummy actually
 *
 * */
#define IO1DIR	IODIR
#define IO1SET	IOSET
#define IO1CLR	IOCLR
#define IO1PIN	IOPIN
#define PINSEL2	PINSEL1

/* 
 * Register Mapping for ADC
 *
 * */
#define AD0CR	ADCR
#define AD0GDR	ADGDR
#endif
