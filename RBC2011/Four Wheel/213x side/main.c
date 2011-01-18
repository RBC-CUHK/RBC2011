#include "armversion.h"
#include "fourwheel.h"

#include "AAL/aal_uart.h"
#include "AAL/aal_pwm.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_spi.h"
#include "AAL/aal_gpio.h"

#include "RAL/ral_motor.h"
#include "RAL/ral_servo.h"
#include "RAL/ral_motor.h"
#include "RAL/ral_mux.h"
#include "RAL/ral_joystick.h"

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

void __irq Timer0_Routine(){
	int AxisLY = Joystick_ReadAxis(LY);
	int AxisLX = Joystick_ReadAxis(LX);
	int AxisRX = Joystick_ReadAxis(RX);
	Fourwheel_Status FWStatus = FREE;
	float SpeedMultiplier = 0;

	
	if(AxisLY > JOYSTICK_UPPERBOUND){
		FWStatus = BACKWARD;
		SpeedMultiplier = (float)((float)AxisLY - JOYSTICK_UPPERBOUND) / (float)(1024.0 - JOYSTICK_UPPERBOUND);
		}
	else if(AxisLY < JOYSTICK_LOWERBOUND){
		FWStatus = FORWARD;
		SpeedMultiplier = (float)(JOYSTICK_LOWERBOUND - (float)AxisLY) / (float)(JOYSTICK_LOWERBOUND - 0.0);
		}
	else if(AxisLX > JOYSTICK_UPPERBOUND){
		FWStatus = RIGHTWARD;
		SpeedMultiplier = (float)((float)AxisLX - JOYSTICK_UPPERBOUND) / (float)(1024.0 - JOYSTICK_UPPERBOUND);
		}
	else if(AxisLX < JOYSTICK_LOWERBOUND){
		FWStatus = LEFTWARD;
		SpeedMultiplier = (float)(JOYSTICK_LOWERBOUND - (float)AxisLX) / (float)(JOYSTICK_LOWERBOUND - 0.0);
		}
	
	if(AxisRX > JOYSTICK_UPPERBOUND){
		FWStatus = ROTATERIGHT;
		SpeedMultiplier = (float)((float)AxisRX - JOYSTICK_UPPERBOUND) / (float)(1024.0 - JOYSTICK_UPPERBOUND);
		}
	else if(AxisRX < JOYSTICK_LOWERBOUND){
		FWStatus = ROTATELEFT;
		SpeedMultiplier = (float)(JOYSTICK_LOWERBOUND - (float)AxisRX) / (float)(JOYSTICK_LOWERBOUND - 0.0);
		}

	if(FWStatus == STOP){
		SpeedMultiplier = 0;
		Fourwheel_Stop();
	} else if(FWStatus == FORWARD){
		Fourwheel_Forward();
	} else if(FWStatus == BACKWARD){
		Fourwheel_Backward();
	} else if(FWStatus == LEFTWARD){
		Fourwheel_Leftward();
	} else if(FWStatus == RIGHTWARD){
		Fourwheel_Rightward();
	} else if(FWStatus == ROTATELEFT){
		Fourwheel_RotateLeft();
	} else if(FWStatus == ROTATERIGHT){
		Fourwheel_RotateRight();
	}
	
	Fourwheel_SetSpeed(SpeedMultiplier);

	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

void __irq Timer1_Routine(){
	int AxisLY = Joystick_ReadAxis(LY);
	int AxisRX = Joystick_ReadAxis(RX);

	Uart_SendInt(AxisLY);
	Uart_SendChar(' ');
	Uart_SendInt(AxisRX);
	Uart_Print("\r\n");

	T1IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}


int main(){
	
	
	//Front Left, Front Right, Back Left, Back Right
	struct Servo_Struct* SS[4] = {&Servo1,&Servo2,&Servo3,&Servo4};	
	struct Motor_Struct* MS[4] = {&Motor1,&Motor2,&Motor3,&Motor4};
	
	
	int muxpin[4] = {19,18,17,16};
	int i,j = 0;
//	int offset = 0;
		
	Uart_Init(57600);
	Mux_Init(&MBMux,20,muxpin);
	Joystick_Init();
	PWM_InitFrequency(50);
	
	//Init the servo motors for angle control
	Motor_Init(&SMotor1,2,99,99,SELF);
	Servo_Init(&Servo1,&SMotor1,13824 + OFFSET1,27648 + OFFSET1,90);
	Motor_Init(&SMotor2,4,99,99,SELF);
	Servo_Init(&Servo2,&SMotor2,13824 + OFFSET2,27648 + OFFSET2,90);
	Motor_Init(&SMotor3,5,99,99,SELF);
	Servo_Init(&Servo3,&SMotor3,13824 + OFFSET3,27648 + OFFSET3,90);
	Motor_Init(&SMotor4,6,99,99,SELF);
	Servo_Init(&Servo4,&SMotor4,13824 + OFFSET4,27648 + OFFSET4,90);
	
	//Init the DC motors for movement
	Motor_Init(&Motor1,1,99,99,MODE2103);
	Motor_Init(&Motor2,2,99,99,MODE2103);
	Motor_Init(&Motor3,3,99,99,MODE2103);
	Motor_Init(&Motor4,4,99,99,MODE2103);


	Fourwheel_Init(SS,MS);

	for (i = 0; i < 1000; i ++)
		for (j = 0; j < 6800; j++);
	SPI_InitMaster(16);
	Timer_Init(0,100,Timer0_Routine);
//	Timer_Init(1,10,Timer1_Routine);
	while(1){
//	//		Following code is used for servo calibration
//		char input;
//		int offset;
//		input = Uart_GetChar();
//		switch(input){
//			case '+' : offset += 10;	break;
//			case '-' : offset -= 10;	break;
//			case '*' : offset += 100;	break;
//			case '/' : offset -= 100;	break;
//		}
//		Servo_Init(&Servo1,&SMotor1,13824 + OFFSET1 + offset,27648 + OFFSET1 + offset,90);
//		Servo_Init(&Servo2,&SMotor2,13824 + OFFSET2 + offset,27648 + OFFSET2 + offset,90);
//		Servo_Init(&Servo3,&SMotor3,13824 + OFFSET3 + offset,27648 + OFFSET3 + offset,90);
//		Servo_Init(&Servo4,&SMotor4,13824 + OFFSET4 + offset,27648 + OFFSET4 + offset,90);
//		Servo_SetAbsolute(&Servo1,45);
//		Servo_SetAbsolute(&Servo2,45);
//		Servo_SetAbsolute(&Servo3,45);
//		Servo_SetAbsolute(&Servo4,45);
//		Uart_SendInt(offset);
//		Uart_Print("\r\n");
	}
	return 0;
}
