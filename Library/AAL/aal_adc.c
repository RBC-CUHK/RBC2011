/**
 *	@file
 *	@brief	AAL_ADC Function Implementation
 * */
#include "armversion.h"
#include "aal_adc.h"

/**
 *	@brief	Init the ADC channel
 *
 *	Perform Pin select
 *
 *	@param	channel	channel	to be inited
 * */
void ADC_InitChannel(int channel){
	switch (channel){
	case 0 : PINSEL1 |= 1 << 22; break;
	case 1 : PINSEL1 |= 1 << 24; break;
	case 2 : PINSEL1 |= 1 << 26; break;
	case 3 : PINSEL1 |= 1 << 28; break;
	case 4 : PINSEL1 |= 1 << 18; break;
	case 5 : PINSEL1 |= 1 << 20; break;
	}
}

/**
 *	@brief	Perfrom Analog to digital convertion
 *
 *	10 bits resolution
 *
 *	@param	channel	channel	to be read
 *
 *	@return	ADC value
 * */
int ADC_Read(int channel){
	AD0CR &= 0x00000000;							//Reset	
	AD0CR |= 1 << channel;                          //Select Analog, 0.0 ~ 0.5
	AD0CR |= (1<<9);								//Set CLKDIV = 0x00000010
	AD0CR |= (1<<21);								//Set AD convertor into operational
	AD0CR |= (1<<24);								//Start the convetor
	while ((AD0GDR & 0x80000000) != 0x80000000);	//Wait until the A/D conversion is completed
	return ((AD0GDR & 0xFFC0) >> 6 );				//Return the Sensor Reading Value
}
