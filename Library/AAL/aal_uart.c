/**
 *	@file
 *	@brief	AAL_UART Function Implementation
 * */
#include "armversion.h"
#include "aal_uart.h"
#include "arm_math.h"

/**
 *	@brief	Init the Uart0
 *	
 *	@param	buadrate	buad rate of the Uart0
 * */
void Uart_Init(int buadrate){
  	/* pin selection for the UART*/
    PINSEL0 |= (1<<0); //TXD for the UART0
    PINSEL0 |= (1<<2); //RxD for the UART0
    
	U0LCR = 0x83;	//8 bit length ,DLAB=1	           
   	U0DLL = (864000/buadrate);	//Fpclk/(16*buadrate);
   	U0DLM = 0x00;
   	U0LCR = 0x03;    //DLAB=0
   	return;
}

/**
 *	@brief	Send out a Interger
 *	
 *	Number of digits of the num is limited by arm_math
 *
 *	@param	num	number to be sent
 *	@see	Uart_SendChar
 *	@see	numDigi
 * */
void Uart_SendInt(int num){
    int digi;
	int i;
	digi=numDigi(num);
	if (num<0){
		Uart_Print("-");
		num = -1*num;
	}	
	for (i=digi-1;i>=0;i--){
	 	Uart_SendChar('0'+(int)(num/pow10(i))%10);
	}	
	return;
}

/**
 *	@brief Send out a character
 *
 *	Underlying function for Uart_Send Family
 *
 *	@param	ch	Character to be sent
 * */
void Uart_SendChar(char ch){ 	/*send an char*/
	while( (U0LSR&0x40)==0 );    
 	U0THR  = ch; // Transmit next character
}

/**
 *	@brief	Send out a string
 *
 *	@param	p	string
 *
 *	@see	Uart_SendChar
 * */
void Uart_Print(char *p){  		/*send a string*/
	while(*p!='\0') {
      Uart_SendChar(*p++);
   }
   return;
}

/**
 *	@brief	Get a character from UART
 *
 *	Blocking call
 *
 *	@return	Charecter read
 * */
char Uart_GetChar(void){ 		/*get a char from the computer*/
	volatile char ch = '0';
  	while ((U0LSR&0x1)==0); 	// wait until  receive a byte
  	ch = U0RBR;        		// receive character
  	return ch;
}

/**
 *	@brief	Send out a 1 digit number
 *	
 *	@param	num	Number to be sent
 *
 *	@see	Uart_SendChar
 * */
void Uart_SendBit(int num){   	/*send a 1 place number*/
     Uart_SendChar('0'+ num);
	 return;
}

/**
 *	@brief	Send out a floating point number
 *
 *	@param	num	Number to be sent
 *	
 *	@see	Uart_SendInt
 *	@see	Uart_SendChar
 * */
void Uart_SendFloat(float num){
	int copy;
	if (num<0){
		Uart_Print("-");
		num = -1*num;
	}
	copy=(int)num; //print the first part as integer
	Uart_SendInt(copy); //print the decimal part
	Uart_SendChar('.');
	Uart_SendChar('0'+(int)(num*10)%10);
	Uart_SendChar('0'+(int)(num*100)%10);
	Uart_SendChar('0'+(int)(num*1000)%10);
	Uart_SendChar('0'+(int)(num*10000)%10);
	
	return;
}

/**
 *	@brief	Send out a fixed length number
 *
 *	Number of digits of the num is limited by arm_math
 *
 *	@param	num		Number to be sent
 *	@param	digit	Number of digits
 *	
 *	@see	Uart_SendChar
 *	@see	pow10
 *
 * */
void Uart_FixSendInt(int num,int digit){	/*send an integer*/
	int _num = num;
	int i;
	if (_num < 0){
		Uart_SendChar('-');
		_num = -1 * _num;
	}else{
		Uart_SendChar('+');
	}	

	for(i = digit; i >= 0; --i){
		Uart_SendChar('0'+ (_num / pow10(i) % 10));	
	}
	return;
}
