/**
 *	@file
 *	@brief	RAL_Joystick Header File
 *
 *	Define ADC channel and GPIO used
 * */
#ifndef	__RAL_JOYSTICK_H
#define __RAL_JOYSTICK_H
// Joystick Analog
// Follow the ADC standard
#define LX 0
#define LY 1
#define RX 2
#define RY 3

// Analog Setting
/// Lower Tolerance which regards as central point
#define JOYSTICK_LOWERBOUND 450.0
/// Upper Tolerance which regards as central point
#define JOYSTICK_UPPERBOUND 525.0

// Joystick Buttons
// Follow the GPIO standard
#define L1 999 
#define L2 999
#define R1 999
#define R2 999
#define B1 122
#define B2 123
#define B3 124
#define B4 125
#define BL 25 
#define BR 26 

void Joystick_Init(void);
int Joystick_ReadAxis(int channel);
int Joystick_ReadButton(int channel);
#endif
