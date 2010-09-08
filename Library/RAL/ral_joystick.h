#ifndef	__RAL_JOYSTICK_H
#define __RAL_JOYSTICK_H
// Jostick
#define RY 3 //ADC 0.3 as RY
#define LX 0 //ADC 0.0 as LX
#define LY 1 //ADC 0.1 as LY
#define RX 2 //ADC 0.2 as RX

#define L1 122 //GPIO 1.22 as L1 input
#define L2 123 //GPIO 1.23 as L2 input
#define R1 124 //GPIO 1.24 as R1 input
#define R2 125 //GPIO 1.25 as R2 input
#define B1 116 //GPIO 1.16 as B1 input
#define B2 21 //GPIO 0.21 as B2 input
#define B3 22 //GPIO 0.22 as B3 input
#define B4 23 //GPIO 0.23 as B4 input
#define BL 25 //GPIO 0.25 as BL input
#define BR 26 //GPIO 0.26 as BR input
// Jostick End

// Analog Setting
#define LOWERBOUND 475.0
#define UPPERBOUND 525.0
// Analog Setting End

void Joystick_Init(void);
int Joystick_ReadAxis(int channel);
int Joystick_ReadButton(int channel);
#endif
