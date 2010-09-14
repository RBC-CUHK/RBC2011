#include "armversion.h"
#include "AAL/aal_uart.h"
#include "RAL/ral_mux.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_adc.h"

unsigned int count = 0;
int CountDecoder = 0;
int CountAnalog = 0;
struct Mux_Struct MuxDecoder;
struct Mux_Struct MuxAnalog;
struct Mux_Struct MBMux;

void __irq Timer0_Routine(){
	int ADCReading;
	if(count % 4 == 0){
	Mux_Set(&MuxDecoder,CountDecoder);
	Mux_Set(&MuxAnalog,CountAnalog);	
	//Convert
	} else if(count % 4 == 1){
		if(CountDecoder < 8)
			ADCReading = ADC_Read(2);
		else
			ADCReading = ADC_Read(3);
		Uart_SendInt(ADCReading);
		Uart_Print("\n\r");
	} else if(count % 4 == 2){
		Mux_Unset(&MuxDecoder);
		Mux_Unset(&MuxAnalog);
		CountDecoder = (CountDecoder + 1) % 16;
		CountAnalog = (CountAnalog + 1) % 8;
	}
	count++;
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){


	
	int GPIODecoder[4] = {26,17,25,16};
	int GPIOAnalog[4] = {99,19,27,18};
	Mux_Init(&MuxDecoder,28,GPIODecoder);
	Mux_Init(&MuxAnalog,99,GPIOAnalog);
	ADC_InitChannel(2);
	ADC_InitChannel(3);	

	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");
	GPIO_Init(31,OUTPUT);
	Timer_Init(0,10,Timer0_Routine);
	while(1);
	return 0;
}
