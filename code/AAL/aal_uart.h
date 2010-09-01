#ifndef __AAL_UART_H
#define __AAL_UART_H

void Uart_Init(int buadrate);	//initialize,default 57600
void Uart_SendInt(int num);   	//send a at most 5 places number
void Uart_SendChar(char ch);	//send an char
void Uart_Print(char *p);  		//send a string
char Uart_GetChar(void);  		//get a char from the computer
void Uart_SendBit(int num);   	//send a 1 place number
void Uart_SendFloat(float num); //send a at most 5 place number and 4 dp
void Uart_FixSendInt(int num);	//send an integer

#endif
