#include "fourwheel.h"

#include "AAL/aal_uart.h"
#include "AAL/aal_213xpwm.h"

#include "RAL/ral_motor.h"
#include "RAL/ral_servo.h"
#include "RAL/ral_motor.h"
#include "RAL/ral_mux.h"

struct Motor_Struct Motor1;
struct Servo_Struct Servo1;
struct Motor_Struct Motor2;
struct Servo_Struct Servo2;
struct Motor_Struct Motor3;
struct Servo_Struct Servo3;
struct Motor_Struct Motor4;
struct Servo_Struct Servo4;

struct Mux_Struct MBMux;

int main(){
	struct Servo_Struct* SS[4] = {&Servo1,&Servo2,&Servo3,&Servo4};
	int count = 0;
	Uart_Init(57600);
	PWM_InitFrequency(50);
	Motor_Init(&Motor1,2,99,99,MODE213x);
	Servo_Init(&Servo1,&Motor1,13824,27648,90);
//	Servo_Init(&Servo1,&Motor1,11700,27500,90);
	Motor_Init(&Motor2,4,99,99,MODE213x);
	Servo_Init(&Servo1,&Motor1,13824,27648,90);
//	Servo_Init(&Servo2,&Motor2,0,0,90);
	Motor_Init(&Motor3,5,99,99,MODE213x);
	Servo_Init(&Servo1,&Motor1,13824,27648,90);
//	Servo_Init(&Servo3,&Motor3,14500,28500,90);
	Motor_Init(&Motor4,6,99,99,MODE213x);
	Servo_Init(&Servo1,&Motor1,13824,27648,90);
//	Servo_Init(&Servo4,&Motor4,13500,28500,90);

	Fourwheel_Init(SS);

	while(1){
		switch(count % 4){
			case 0 : Fourwheel_Forward();
			case 1 : Fourwheel_Leftward();
			case 2 : Fourwheel_Backward();
			case 3 : Fourwheel_Rightward();
			break;
		}
		Uart_GetChar();
		count++;
	}
	return 0;
}
