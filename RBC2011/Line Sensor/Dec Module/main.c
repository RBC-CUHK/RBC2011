#include "armversion.h"
#include "AAL/aal_uart.h"
#include "RAL/ral_mux.h"
#include "RAL/ral_motor.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_pwm.h"
#include "RAL/ral_linesensor.h"

int counter = 0;
//int counter2 = 0;

struct Mux_Struct MBMux;

struct Motor_Struct LeftMotor;
struct Motor_Struct RightMotor;

void __irq Timer0_Routine(){
	Linesensor_Update();
	
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

void __irq Timer1_Routine(){
//	int combined = Linesensor_ReadAll();
//	int straight = combined & 0xff;
	
	T1IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){
	int LEDPins[12] = {99};
	int TransistorPins[12] = {99};

	Linesensor_Init(TransistorPins,LEDPins);

	PWM_InitPeriod(2000);
	
	Motor_Init(&LeftMotor,2,16,17,SELF);
	Motor_Init(&RightMotor,5,18,19,SELF);

	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");

	Timer_Init(0,20000,Timer0_Routine);
	Timer_Init(1,50,Timer1_Routine);

	Motor_SetPWM(&LeftMotor,1000);
	Motor_SetPWM(&RightMotor,-1000);
	while(1);	   
	return 0;
}
