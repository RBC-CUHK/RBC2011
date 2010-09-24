#include "armversion.h"

#include "AAL/aal_spi.h"
#include "AAL/aal_uart.h"
#include "AAL/aal_pwm.h"

#include "RAL/ral_mux.h"
#include "RAL/ral_motor.h"


struct Mux_Struct MBMux;

struct Motor_Struct Motor[10];

static unsigned short spi_get_data = 0;
static unsigned short spi_send_data = 0;

void __irq SPI_Routine(){
	int input;
	int motor;
	int level;
	char sign;

	if ((S0SPSR & 0xF8) == 0x80){
		input = S0SPDR; // read byte from master, clear the flag
	}
	spi_get_data = input;

	// We need to set the PWM here
	motor = (spi_get_data >> 12);
	level = (spi_get_data & 0x07FF);
	sign = (spi_get_data & 0x0800) >> 11;
	if (sign == 1)
		level *= -1;
				
	S0SPDR = spi_send_data;
	
	Motor_SetPWM(&Motor[motor],level);
	
//	Uart_Print("SetPWM-");
	Uart_SendInt(motor);
//	Uart_SendChar(':');
	Uart_SendInt(level);
//	Uart_Print("\r\n");
	
	S0SPINT = 0x01; // reset interrupt flag
	VICVectAddr = 0; // reset VIC
}

int main(){
	Uart_Init(57600);
	Uart_Print("Uart Inited\r\n");
	Motor_Init(&Motor[0],0,17,99,SELF);
	Motor_Init(&Motor[1],21,18,99,SELF);
	Motor_Init(&Motor[2],22,22,99,SELF);
	Motor_Init(&Motor[3],2,25,99,SELF);
	Motor_Init(&Motor[4],11,24,99,SELF);
	Motor_Init(&Motor[5],10,23,99,SELF);
	Motor_Init(&Motor[6],12,26,99,SELF);
	Motor_Init(&Motor[7],44,99,99,SELF);
	Motor_Init(&Motor[8],30,28,99,SELF);
	Motor_Init(&Motor[9],33,29,99,SELF);
	Uart_Print("Motor Inited\r\n");
	PWM_InitPeriod(2000);
	SPI_InitSlave(16,SPI_Routine);
	Uart_Print("SPI Inited\r\n");
	while(1);
	return 0;
}
