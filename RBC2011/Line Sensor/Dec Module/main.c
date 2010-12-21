#include "armversion.h"
#include "AAL/aal_uart.h"
#include "RAL/ral_mux.h"
#include "RAL/ral_motor.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_adc.h"
#include "AAL/aal_pwm.h"

int counter = 0;
int counter2 = 0;
int GPIOOutput[4] = {9,8,22,6};
int GPIOInput[4] = {5,4,26,25};
int GPIOReading[4] = {0};

void __irq Timer0_Routine(){
	int i;
	if(counter < 4){
		GPIO_Set(GPIOOutput[counter],1);
		GPIO_Set(GPIOOutput[(counter + 3) % 4],0);
	} 
	if(counter == 4)
		GPIO_Set(6,0);
	if((counter >= 2) && (counter <= 5))
		GPIOReading[(counter + 2) % 4] = GPIO_Read(GPIOInput[(counter + 2) % 4]);
	
	++counter;
	++counter2;
	if(counter2 > 120){
		for(i = 0; i < 4; ++i){
			Uart_SendInt(GPIOReading[i]);
			Uart_SendChar(' ');
		}
		Uart_Print("\r\n");
		counter2 = 0;	
	}
	if(counter > 12) counter = 0;
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

void __irq Timer1_Routine(){
	int i;
	Uart_Print("Hello\r\n");
	T1IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){
	int GPIODecoder[4] = {16,25,17,26};
	int GPIOAnalog[4] = {99,12,13,14};
	struct Motor_Struct MS[4];
	int i,j;
	GPIO_Init(9,OUTPUT);
	GPIO_Init(8,OUTPUT);
	GPIO_Init(22,OUTPUT);
	GPIO_Init(6,OUTPUT);


	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");
	Timer_Init(0,24000,Timer0_Routine);
	while(1);
	return 0;
}
