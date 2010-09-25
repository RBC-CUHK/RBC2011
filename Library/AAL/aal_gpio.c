/**
 *	@file
 *	@brief	AAL_GPIO Function Implementation
 * */
#include "armversion.h"
#include "aal_gpio.h"

/**
 *	@brief	Init the GPIO pin
 *
 *	Do pin select and set the direction
 *
 *	@param 	GPIO		GPIO Standadrd
 *	@param	direction	INPUT / OUTPUT
 * */
void GPIO_Init(int GPIO, int direction){
	int port = GPIO / 100;
	int channel = GPIO % 100;
	switch (port){
		   int bitmask;
		   case 0:  bitmask = ~(0x3 << (channel % 16 * 2));
		   		    if (channel <= 15)
				   	   PINSEL0 &= bitmask;
				    else
				   	   PINSEL1 &= bitmask;
				    if (direction == INPUT)
				   	   IO0DIR &= (0xFFFFFFFF & (~(1 << channel)));
	   			    else
				   	   IO0DIR |= 1 << channel;
					break;
					
			case 1: if ((channel >= 16) && (channel <= 25)) 
				 	   PINSEL2 &= 0xFFFFFFF7;
				 	else
					   PINSEL2 &= 0xFFFFFFFB;
				 	if (direction == INPUT)
					   IO1DIR &= (0xFFFFFFFF & (~(1 << channel)));
					else
						IO1DIR |= 1 << channel;
					break;
	}
	
}

/**
 *	@brief	Read the GPIO
 *
 *	@param	GPIO	GPIO Standard
 *
 *	@return	GPIO value read, -1 indicates invalid pin
 * */
int GPIO_Read(int GPIO){
	int port = GPIO / 100;
	int channel = GPIO % 100;
	if(channel > 32) return -1;
	switch(port){
		 case 0: return (IO0PIN >> channel) & 0x1; 
		 case 1: return (IO1PIN >> channel) & 0x1;	
	}
	return -1;
}

/**
 *	@brief	Set the GPIO pin
 *
 *	@param	GPIO	GPIO Standard
 *	@param	value	0 / 1
 * */
void GPIO_Set(int GPIO, int value){
	int port = GPIO / 100;
	int channel = GPIO % 100;
	if(channel > 32) return ;
	switch(port){
       case 0: switch(value){
				   case 0: IO0CLR |= (1 << channel); break;
				   case 1: IO0SET |= (1 << channel); break;
			   }
			   break;
	   case 1: switch(value){
				   case 0: IO1CLR |= (1 << channel); break;
				   case 1: IO1SET |= (1 << channel); break;
			   }
	   break;
	}
}
