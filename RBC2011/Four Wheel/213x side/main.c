#include "armversion.h"
#include "fourwheel.h"

#include "AAL/aal_uart.h"
#include "AAL/aal_213xpwm.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_spi.h"

#include "RAL/ral_motor.h"
#include "RAL/ral_servo.h"
#include "RAL/ral_motor.h"
#include "RAL/ral_mux.h"

struct Motor_Struct SMotor1;
struct Servo_Struct Servo1;
struct Motor_Struct SMotor2;
struct Servo_Struct Servo2;
struct Motor_Struct SMotor3;
struct Servo_Struct Servo3;
struct Motor_Struct SMotor4;
struct Servo_Struct Servo4;

struct Motor_Struct Motor1;
struct Motor_Struct Motor2;
struct Motor_Struct Motor3;
struct Motor_Struct Motor4;

struct Mux_Struct MBMux;

int count = 0;
int pcount = 0;

void __irq Timer0_Routine(){

	
	
	switch (count % 6){
		case 0 : Fourwheel_Forward();		break;
		case 1 : Fourwheel_Leftward();		break;
		case 2 : Fourwheel_Backward();		break;
		case 3 : Fourwheel_Rightward();		break;
		case 4 : Fourwheel_RotateRight();	break;
		case 5 : Fourwheel_RotateLeft();	break;
	}
	pcount++;
	count = pcount / 100;
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}


int main(){
	struct Servo_Struct* SS[4] = {&Servo1,&Servo2,&Servo3,&Servo4};
	struct Motor_Struct* MS[4] = {&Motor1,&Motor2,&Motor3,&Motor4};
	int muxpin[4] = {19,18,17,16};
		
	Uart_Init(57600);
	Mux_Init(&MBMux,20,muxpin);
	PWM_InitFrequency(50);
	
	//Init the servo motors for angle control
	Motor_Init(&SMotor1,2,99,99,MODE213x);
	Servo_Init(&Servo1,&SMotor1,13824,27648,90);
	Motor_Init(&SMotor2,4,99,99,MODE213x);
	Servo_Init(&Servo2,&SMotor2,13824,27648,90);
	Motor_Init(&SMotor3,5,99,99,MODE213x);
	Servo_Init(&Servo3,&SMotor3,13824,27648,90);
	Motor_Init(&SMotor4,6,99,99,MODE213x);
	Servo_Init(&Servo4,&SMotor4,13824,27648,90);
	
	//Init the DC motors for movement
	Motor_Init(&Motor1,1,99,99,MODE2103);
	Motor_Init(&Motor2,2,99,99,MODE2103);
	Motor_Init(&Motor3,3,99,99,MODE2103);
	Motor_Init(&Motor4,4,99,99,MODE2103);


	Fourwheel_Init(SS,MS);
	SPI_InitMaster(16);
	Timer_Init(0,10,Timer0_Routine);
	//Fourwheel_RotateLeft();
//	Motor_SetPWM(&Motor1,20736);
//	Motor_SetPWM(&Motor2,20736);
//	Motor_SetPWM(&Motor3,20736);
//	Motor_SetPWM(&Motor4,20736);
//	while(1);
	while(1){

	}
	return 0;
}
