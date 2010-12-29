#include "armversion.h"

#include "AAL/aal_uart.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_pwm.h"

#include "RAL/ral_mux.h"
#include "RAL/ral_encoder.h"
#include "RAL/ral_motor.h"
#include "RAL/ral_pid.h"
#include "RAL/ral_odometry.h"

struct Mux_Struct MBMux;

struct Motor_Struct LeftMotor;
struct Motor_Struct RightMotor;

struct PIDStruct LMotorPID;
struct PIDStruct RMotorPID;
struct PIDStruct ThetaPID;

int lold,lcur,rold,rcur,lcount,rcount = 0;
int counter = 0;

void __irq Timer0_Routine(){
	lcur = GPIO_Read(6);
	rcur = GPIO_Read(22);

	if(lcur!=lold) {		// increment left counter
		lcount++;
		lold=lcur;
	}

	if(rcur!=rold) {		// increment right counter
		rcount++;
		rold=rcur;
	}

	Encoder_Set(0,lcount);
	Encoder_Set(1,rcount);
	
	++counter;
	if(counter == 10){
		Odometry_Update();
		counter = 0;
	}
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

void __irq Timer1_Routine(){
	
	PID_UpdateAll();

	T1IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){
	Uart_Init(57600);
	Uart_Print("Hello World!\r\n");

	PWM_InitFrequency(50);

	Motor_Init(&LeftMotor,2,16,17,SELF);
	Motor_Init(&RightMotor,5,18,19,SELF);

	//P0.6 Left Wheel Sensor
	GPIO_Init(6,INPUT);
	//P0.22 Right Wheel Sensor
	GPIO_Init(22,INPUT);

	PID_Init_RealVel(&LMotorPID,0.2,88,100,4,&LeftMotor,0,1);
	PID_Init_RealVel(&RMotorPID,0.2,88,100,4,&RightMotor,1,1);

	PID_SetParameter(&LMotorPID,8000,6000,5000,100000,100);
	PID_SetParameter(&RMotorPID,8000,6000,5000,100000,100);

	Odometry_Init(0,1);

	PID_Start(&LMotorPID);
	PID_Start(&RMotorPID);

	Timer_Init(0,1000,Timer0_Routine);
	Timer_Init(0,4,Timer1_Routine);

	while(1);
	return 0;
}
