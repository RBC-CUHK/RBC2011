/**
 *	@file
 *	@brief	AAL_GPIO Header File
 * */
#ifndef __AAL_GPIO_H
#define __AAL_GPIO_H

///	GPIO as Input pin
#define	INPUT	0
///	GPIO as Output pin
#define OUTPUT	1

void GPIO_Init(int GPIO, int direction);
int GPIO_Read(int GPIO);
void GPIO_Set(int GPIO, int value);

#endif
