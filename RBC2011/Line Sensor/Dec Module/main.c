#include "armversion.h"
#include "AAL/aal_uart.h"
#include "RAL/ral_mux.h"
#include "RAL/ral_motor.h"
#include "AAL/aal_timer.h"
#include "AAL/aal_gpio.h"
#include "AAL/aal_adc.h"
#include "AAL/aal_pwm.h"

int counter = 0;
//int counter2 = 0;

struct Mux_Struct MBMux;

int GPIOOutput[4] = {116,117,118,119};
int GPIOInput[4] = {120,121,122,123};
int GPIOReading[4] = {0};

struct Motor_Struct LeftMotor;
struct Motor_Struct RightMotor;

void __irq Timer0_Routine(){
	int readchannel;
	if(counter < 4){
		GPIO_Set(GPIOOutput[counter],1);		
		GPIO_Set(GPIOOutput[(counter + 3) % 4],0);
	} 
	else if(counter == 4)
		GPIO_Set(GPIOOutput[3],0);
	if((counter >= 2) && (counter <= 5)){
		readchannel = (counter + 2) % 4;
		GPIOReading[readchannel] = GPIO_Read(GPIOInput[readchannel]);
	}

	++counter;

	if(counter == 10) counter = 0;
	
	T0IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

void __irq Timer1_Routine(){
	
	int combined = 0;
//	int i;
//	for(i = 0; i < 4; ++i){
//		Uart_SendInt(GPIOReading[i]);
//		Uart_SendChar(' ');
//	}
//	Uart_Print("\r\n");
	
	combined = GPIOReading[0] | (GPIOReading[1] << 1) | (GPIOReading[2] << 2) | (GPIOReading[3] << 3);

	switch(combined){
		case 0xf	:	Motor_SetPWM(&LeftMotor,1000);
						Motor_SetPWM(&RightMotor,-1000);	
						break;
		case 0xe	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM + 10);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM - 10));
						break;
		case 0xd	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM - 10);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM + 10));
						break;
		case 0xc	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM - 10);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM + 10));
						break;
		case 0xb	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM - 10);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM + 10));
						break;
		//1 3 unsure case, let it go
		case 0xa	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM * 1);
						Motor_SetPWM(&RightMotor,-RightMotor.currPWM * 1);
						break;
		case 0x9	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM - 20);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM + 20));
						break;
		case 0x8	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM + 20);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM - 20));
						break;
		case 0x7	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM + 10);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM - 10));
						break;
		case 0x6	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM + 20);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM - 20));
						break;
		//0 2 unsure case, let it go
		case 0x5	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM * 1);
						Motor_SetPWM(&RightMotor,-RightMotor.currPWM * 1);
						break;
		case 0x4	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM - 20);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM + 20));
						break;
		case 0x3	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM + 10);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM - 10));
						break;
		case 0x2	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM - 20);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM + 20));
						break;
		case 0x1	:	Motor_SetPWM(&LeftMotor,LeftMotor.currPWM + 20);
						Motor_SetPWM(&RightMotor,-(RightMotor.currPWM - 20));
						break;
		case 0x0	:	Motor_SetPWM(&LeftMotor,1000);
						Motor_SetPWM(&RightMotor,-1000);
						break;
	}
	T1IR = 1;                              	// Clear interrupt flag
	VICVectAddr = 0;                       	// Acknowledge Interrupt
}

int main(){
	int i;


	PWM_InitPeriod(2000);
	
	Motor_Init(&LeftMotor,2,16,17,SELF);
	Motor_Init(&RightMotor,5,18,19,SELF);
	
	Motor_SetPWM(&LeftMotor,1000);
	Motor_SetPWM(&RightMotor,-1000);
	

	for(i = 0; i < 4; ++i){
		GPIO_Init(GPIOInput[i],INPUT);
		GPIO_Init(GPIOOutput[i],OUTPUT);
	}

	Uart_Init(57600);
	Uart_Print("Uart Inited\n\r");

	Timer_Init(0,20000,Timer0_Routine);
	Timer_Init(1,50,Timer1_Routine);
	while(1);	   
	return 0;
}
