#include "AAL/aal_uart.h"

int main(){
	Uart_Init(57600);
	Uart_Print("Hello");
	return 0;
}
