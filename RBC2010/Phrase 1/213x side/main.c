#include "AAL/aal_uart.h"
#include "RAL/ral_encoder.h"
#include "RAL/ral_mux.h"
#include "AAL/aal_spi.h"

int main(){
	int muxpin[4] = {19,18,17,16};
	struct Mux_Struct MBMux;

	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");
	SPI_InitMaster(16);

	Mux_Init(&MBMux,20,muxpin);
	Encoder_Init(&MBMux);
	Uart_Print("SPI Encoder Inited\n\r");

	return 0;
}
