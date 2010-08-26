#ifndef __AAL_GPIO_H
#define __AAL_GPIO_H

void GPIO_Init(float GPIO,int direction);	//INPUT / OUTPUT
int GPIO_Read(float GPIO);
void GPIO_Set(float int GPIO, int value);

#endif
