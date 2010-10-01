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
	int ButtonB1 = Joystick_ReadButton(B1);
	int ButtonB2 = Joystick_ReadButton(B2);
	int ButtonB3 = Joystick_ReadButton(B3);
	int ButtonB4 = Joystick_ReadButton(B4);
	int ButtonBL = Joystick_ReadButton(BL);
	int ButtonBR = Joystick_ReadButton(BR);
	int AxisLY = Joystick_ReadAxis(LY);

//	Uart_SendInt(ButtonB1);
//	Uart_SendInt(ButtonB2);
//	Uart_SendInt(ButtonB3);
//	Uart_SendInt(ButtonB4);
//	Uart_SendInt(ButtonBL);
//	Uart_SendInt(ButtonBR);
//	Uart_Print("\r\n");
	
	if(ButtonB1)
		Fourwheel_Forward();
	else if(ButtonB2)
		Fourwheel_Rightward();
	else if(ButtonB3)
		Fourwheel_Backward();
	else if(ButtonB4)
		Fourwheel_Leftward();
	else if(ButtonBL)
		Fourwheel_RotateLeft();
	else if(ButtonBR)
		Fourwheel_RotateRight();
	else
		Fourwheel_Stop();
	
	if(AxisLY > JOYSTICK_UPPERBOUND)
		Fourwheel_SetSpeed(MAXSPEED * (AxisLY - JOYSTICK_UPPERBOUND) / (1024.0 - JOYSTICK_UPPERBOUND));
	else if(AxisLY < JOYSTICK_LOWERBOUND)
		Fourwheel_SetSpeed(MAXSPEED * (JOYSTICK_LOWERBOUND - AxisLY) / (JOYSTICK_LOWERBOUND - 0.0));
	else
		Fourwheel_SetSpeed(0);
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}


int main(){
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
	Timer_Init(0,1000,Timer0_Routine);
	
	while(1){
//		char input;
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
//		Uart_SendInt(offset);
//		Uart_Print("\r\n");
	}
	return 0;
}
