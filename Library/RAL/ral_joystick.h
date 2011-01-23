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
#define LX 4
#define LY 5
#define RX 2
#define RY 3

// Analog Setting
/// Lower Tolerance which regards as central point
#define JOYSTICK_LOWERBOUND 425.0
/// Upper Tolerance which regards as central point
#define JOYSTICK_UPPERBOUND 525.0

// Joystick Buttons
// Follow the GPIO standard
#define L1 124 
#define L2 125
#define R1 22
#define R2 23
#define B1 999
#define B2 122
#define B3 999
#define B4 123
#define BL 999 
#define BR 999 

void Joystick_Init(void);
int Joystick_ReadAxis(int channel);
int Joystick_ReadButton(int channel);
#endif
