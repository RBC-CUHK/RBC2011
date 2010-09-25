/**
 *	@file
 *	@brief	AAL_UART Header File
 * */
#ifndef __AAL_UART_H
#define __AAL_UART_H

void Uart_Init(int buadrate);	
void Uart_SendInt(int num);   	
void Uart_SendChar(char ch);	
void Uart_Print(char *p);  		
char Uart_GetChar(void);  	
void Uart_SendBit(int num); 
void Uart_SendFloat(float num);
void Uart_FixSendInt(int num,int digit);	

#endif
