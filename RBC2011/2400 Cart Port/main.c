#include "armversion.h"

#include <stdlib.h>
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

extern struct Pos Odometry_Pos;
int lold,lcur,rold,rcur,lcount,rcount = 0;
int counter = 0;

void __irq Timer0_Routine(){
	lcur = GPIO_Read(5);
	rcur = GPIO_Read(6);

	if(lcur!=lold) {		// increment left counter
		if(LeftMotor.currPWM > 0)
			lcount++;
		else
			lcount--;
		lold=lcur;
	}

	if(rcur!=rold) {		// increment right counter
		if(RightMotor.currPWM > 0)
			rcount++;
		else
			rcount--;
		rold=rcur;
	}

	Encoder_Set(0,lcount);
	Encoder_Set(1,rcount);
	
	++counter;
	if(counter == 50){
		Odometry_Update();
		counter = 0;
	}
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

void __irq Timer1_Routine(){
	
	PID_UpdateAll();
	Uart_SendFloat(Odometry_Pos.theta);
	Uart_SendChar(' ');
//	Uart_SendFloat(LMotorPID.info.Vel.targetSpeed);
//	Uart_SendChar(' ');
//	Uart_SendFloat(RMotorPID.info.Vel.targetSpeed);
//	Uart_SendInt(LeftMotor.currPWM);
//	Uart_SendChar(' ');
//	Uart_SendInt(RightMotor.currPWM);
	Uart_SendInt(RMotorPID.info.Vel.currentSpeed);
	Uart_SendChar('/');
	Uart_SendFloat(RMotorPID.info.Vel.targetSpeed);
//	Uart_SendChar('-');
//	Uart_SendInt(RightMotor.currPWM);
	Uart_Print("\r\n");
	
	T1IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){
	Uart_Init(57600);
	Uart_Print("Hello World!\r\n");

	PWM_InitFrequency(50);

	Motor_Init(&LeftMotor,2,16,17,SELF);
	Motor_Init(&RightMotor,5,19,18,SELF);

	//P0.5 Left Wheel Sensor
	GPIO_Init(5,INPUT);
	//P0.6 Right Wheel Sensor
	GPIO_Init(6,INPUT);

	PID_Init_RealVel(&LMotorPID,8,88,18.8,1,&LeftMotor,0,1);
	PID_Init_RealVel(&RMotorPID,8,88,18.8,1,&RightMotor,1,1);
	PID_Init_Theta(&ThetaPID,0,&LMotorPID.info.Vel,&RMotorPID.info.Vel,&Odometry_Pos,1);

	PID_SetParameter(&LMotorPID,2000,500,0,100000,0.5);
	PID_SetParameter(&RMotorPID,2000,500,0,100000,0.5);
	PID_SetParameter(&ThetaPID,0.5,0,0,0,0.1);

	Odometry_Init(0,1);

	PID_Push(&LMotorPID);
	PID_Push(&RMotorPID);
	PID_Push(&ThetaPID);

	PID_Start(&LMotorPID);
	PID_Start(&RMotorPID);
	PID_Start(&ThetaPID);

	Timer_Init(0,1000,Timer0_Routine);
	Timer_Init(1,1,Timer1_Routine);

	while(1);
	return 0;
}

