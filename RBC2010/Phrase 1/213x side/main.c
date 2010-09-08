#include "armversion.h"

#include "AAL/aal_uart.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_spi.h"

#include "RAL/ral_encoder.h"
#include "RAL/ral_mux.h"
#include "RAL/ral_joystick.h"


struct Mux_Struct MBMux;

void __irq	Timer_Rountine(){
	GPIO_Set(31,1);
	GPIO_Set(31,0);
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){
	int muxpin[4] = {19,18,17,16};

	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");

	SPI_InitMaster(16);
	Mux_Init(&MBMux,20,muxpin);
	Encoder_Init(&MBMux);
	Uart_Print("SPI Encoder Inited\n\r");

	Joystick_Init();
	GPIO_Init(31,OUTPUT);

	Timer_Init(0,100,Timer_Rountine);
	return 0;
}
