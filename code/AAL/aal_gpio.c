#include <lpc213x.h>
#include "aal_gpio.h"

//void GPIO_Init(float GPIO, int direction){
//	int port = (int)GPIO;
//	int channel =(GPIO-port)*100;
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
				    if (direction == OUTPUT)
				   	   IO0DIR &= (0xFFFFFFFF & (~(1 << channel)));
	   			    else
				   	   IO0DIR |= 1 << channel;
					break;
					
			case 1: if ((channel >= 16) && (channel <= 25)) 
				 	   PINSEL2 &= 0xFFFFFFF7;
				 	else
					   PINSEL2 &= 0xFFFFFFFB;
				 	if (direction == OUTPUT)
					   IO1DIR &= (0xFFFFFFFF & (~(1 << channel)));
					else
						IO1DIR |= 1 << channel;
					break;
	}
	
}

//int GPIO_Read(float GPIO){
//	int port = (int)GPIO;
//	int channel =(GPIO-port)*100;
int GPIO_Read(int GPIO){
	int port = GPIO / 100;
	int channel = GPIO % 100;
	switch(port){
		 case 0: return (IO0PIN >> channel) & 0x1; 
		 case 1: return (IO1PIN >> channel) & 0x1;	
	}
	return 0;
}

//void GPIO_Set(float GPIO, int value){
//	int port = (int)GPIO;
//	int channel =(GPIO-port)*100;
void GPIO_Set(int GPIO, int value){
	int port = GPIO / 100;
	int channel = GPIO % 100;
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
