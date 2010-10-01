#include "armversion.h"
#include "AAL/aal_uart.h"
#include "RAL/ral_mux.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_adc.h"

#define DELAY	20

unsigned int count = 0;
int CountDecoderArray[16] = {0,7,1,8,2,9,3,10,4,11,5,12,6,13,14,15};
//int CountDecoderArray[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int CountAnalogArray[8] = {0,1,2,3,4,5,6,7};
int CountDecoder = 0;
int CountAnalog = 0;
char timerflag = 0;
struct Mux_Struct MuxDecoder;
struct Mux_Struct MuxAnalog;
struct Mux_Struct MBMux;
int ADCReading[16] = {0};

void __irq Timer0_Routine(){
	int delay;

	if(timerflag == 0){
		timerflag = 1;
//		GPIO_Set(23,0);
	} else {
		timerflag = 0;
//		GPIO_Set(23,1);
	}
	Mux_Set(&MuxDecoder,CountDecoderArray[CountDecoder]);
	Mux_Set(&MuxAnalog,CountAnalogArray[CountAnalog]);
	for(delay = 0 ; delay < DELAY ; delay++);	
	//Convert
	GPIO_Set(31,1);
	if(CountDecoder < 8)
		ADCReading[CountDecoderArray[CountDecoder]] = ADC_Read(1);
	else
		ADCReading[CountDecoderArray[CountDecoder]] = ADC_Read(2);
	
//	Uart_SendInt(ADCReading[CountDecoder]);
//	Uart_SendChar(' ');
	GPIO_Set(31,0);
	for(delay = 0 ; delay < DELAY ; delay++);
	Mux_Unset(&MuxDecoder);
	Mux_Unset(&MuxAnalog);

//	Uart_SendInt(CountDecoderArray[CountDecoder]);
//	Uart_SendChar('-');
//	Uart_SendInt(CountAnalogArray[CountAnalog]);
//	Uart_Print("\r\n");
	CountDecoder = (CountDecoder + 1) % 16;
	CountAnalog = (CountAnalog + 1) % 8;	
	for(delay = 0; delay < 6; delay++){
		Uart_SendInt(ADC_Read(delay));
		Uart_SendChar(' ');
	}
	Uart_Print("\r\n");
//	if(CountDecoder == 8)
//		Uart_Print("\r\n");
//	if(CountDecoder == 0)
//		Uart_Print("\r\n***\r\n"); 

	
	//count++;
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){


	
	int GPIODecoder[4] = {16,25,17,26};
	int GPIOAnalog[4] = {99,12,13,14};
	Mux_Init(&MuxDecoder,23,GPIODecoder);
	Mux_Init(&MuxAnalog,99,GPIOAnalog);
//	ADC_InitChannel(0);
	ADC_InitChannel(1);
	ADC_InitChannel(2);
//	ADC_InitChannel(3);
//	ADC_InitChannel(4);
//	ADC_InitChannel(5);

	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");
	GPIO_Init(31,OUTPUT);
//	GPIO_Init(23,OUTPUT);
	Timer_Init(0,1000,Timer0_Routine);
	while(1);
	return 0;
}
