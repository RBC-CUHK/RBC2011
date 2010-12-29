/**
 *	@file
 *	@brief	RAL_Linesensor Function Implementation
 * */

#include "ral_linesensor.h"

#include "../AAL/aal_gpio.h"

static int _InputPins[12] = {99};
static int _OutputPins[12] = {99};
static int _Readings[12] = {-1};
static int _counter = 0;

void Linesensor_Init(int InputPins[12], int OutputPins[12]){
	int i;
	for(i = 0; i < 12; ++i){
		_OutputPins[i] = OutputPins[i];
		GPIO_Init(OutputPins[i],OUTPUT);
		_InputPins[i] = InputPins[i];
		GPIO_Init(InputPins[i],INPUT);
	}
	_counter = 0;
	return;
}


void Linesensor_Update(void){
	++_counter;
	switch(_counter){
		case 1	:	GPIO_Set(_OutputPins[0],1);
					GPIO_Set(_OutputPins[2],1);
					GPIO_Set(_OutputPins[6],1);
					GPIO_Set(_OutputPins[11],1);
					break;
		case 2	:	GPIO_Set(_OutputPins[0],0);
					GPIO_Set(_OutputPins[2],0);
					GPIO_Set(_OutputPins[6],0);
					GPIO_Set(_OutputPins[11],0);
					break;
		case 3	:	_Readings[0] = GPIO_Read(_InputPins[0]);
					_Readings[2] = GPIO_Read(_InputPins[2]);
					_Readings[6] = GPIO_Read(_InputPins[6]);
					_Readings[11] = GPIO_Read(_InputPins[11]);
					GPIO_Set(_OutputPins[3],1);
					GPIO_Set(_OutputPins[5],1);
					GPIO_Set(_OutputPins[8],1);
					GPIO_Set(_OutputPins[10],1);
					break;
		case 4	:	GPIO_Set(_OutputPins[3],0);
					GPIO_Set(_OutputPins[5],0);
					GPIO_Set(_OutputPins[8],0);
					GPIO_Set(_OutputPins[10],0);
					break;
		case 5	:	_Readings[3] = GPIO_Read(_InputPins[3]);
					_Readings[5] = GPIO_Read(_InputPins[5]);
					_Readings[8] = GPIO_Read(_InputPins[8]);
					_Readings[10] = GPIO_Read(_InputPins[10]);
					GPIO_Set(_OutputPins[1],1);
					GPIO_Set(_OutputPins[4],1);
					GPIO_Set(_OutputPins[7],1);
					GPIO_Set(_OutputPins[9],1);
					break;
		case 6	:	GPIO_Set(_OutputPins[1],0);
					GPIO_Set(_OutputPins[4],0);
					GPIO_Set(_OutputPins[7],0);
					GPIO_Set(_OutputPins[9],0);
					break;
		case 7	:	_Readings[1] = GPIO_Read(_InputPins[1]);
					_Readings[4] = GPIO_Read(_InputPins[4]);
					_Readings[7] = GPIO_Read(_InputPins[7]);
					_Readings[9] = GPIO_Read(_InputPins[9]);
					break;
		case 10	:	_counter = 0;
					break;
	}
	return;
}

int Linesensor_Read(int channel){
	return _Readings[channel];
}

int Linesensor_ReadAll(void){
	int temp = 0;
	int i;
	for( i = 0; i < 12; ++i)
		temp = temp | (_Readings[i] << i);
	return temp;
}

int Linesensor_ReadAllTrans(void){
	return Linesensor_ReadAll();	
}
