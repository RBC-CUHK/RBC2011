#ifndef __AAL_GPIO_H
#define __AAL_GPIO_H

#define	INPUT	0
#define OUTPUT	1

//Float type has precision problem would actually cause problems
//(test case 0.7 would give 0 69 in my C program in linux)
//would switch back to int --Wilson

//void GPIO_Init(float GPIO,int direction);	//INPUT / OUTPUT
//int GPIO_Read(float GPIO);
//void GPIO_Set(float int GPIO, int value);

void GPIO_Init(int GPIO, int direction);
int GPIO_Read(int GPIO);
void GPIO_Set(int GPIO, int value);

#endif
