#include "armversion.h"

#include "AAL/aal_uart.h"

#include "RAL/ral_mux.h"


struct Mux_Struct MBMux;

int main(){
	Uart_Init(57600);
	Uart_Print("Hello World!\r\n");
	return 0;
}
