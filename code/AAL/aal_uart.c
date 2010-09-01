#include <lpc213x.h>
#include "aal_uart.h"
#include "arm_math.h"

void Uart_Init(int buadrate){
  	/* pin selection for the UART*/
    PINSEL0 |= (1<<0); //TXD for the UART0
    PINSEL0 |= (1<<2); //RxD for the UART0
    
	U0LCR = 0x83;	//8 bit length ,DLAB=1	           
   	U0DLL = 3+(13.824*1000000/(16*buadrate));	//3+Fpclk/(16*buadrate);
   	U0DLM = 0x00;
   	U0LCR = 0x03;    //DLAB=0
   	return;
}

void Uart_SendInt(int num){   	/*send a at most 5 places number*/
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

void Uart_SendChar(char ch){ 	/*send an char*/
	while( (U0LSR&0x40)==0 );    
 	U0THR  = ch; // Transmit next character
}

void Uart_Print(char *p){  		/*send a string*/
	while(*p!='\0') {
      Uart_SendChar(*p++);
   }
   return;
}

char Uart_GetChar(void){ 		/*get a char from the computer*/
	volatile char ch = '0';
  	while ((U0LSR&0x1)==0); 	// wait until  receive a byte
  	ch = U0RBR;        		// receive character
  	return ch;
}

void Uart_SendBit(int num){   	/*send a 1 place number*/
     Uart_SendChar('0'+ num);
	 return;
}

void Uart_SendFloat(float num){ /*send a at most 5 place number and 4 dp*/
	int copy;
	if (num<0){
		Uart_Print("-");
		num = -1*num;
	}
	copy=(int)num; //print the first part as integer
	Uart_SendInt(copy); //print the decimal part
	Uart_Print(".");
	Uart_SendChar('0'+(int)(num*10)%10);
	Uart_SendChar('0'+(int)(num*100)%10);
	Uart_SendChar('0'+(int)(num*1000)%10);
	Uart_SendChar('0'+(int)(num*10000)%10);
	
	return;
}

void Uart_FixSendInt(int num){	/*send an integer*/
	int _num = num;

	if (_num < 0){
		Uart_Print("-");
		_num = -1 * _num;
	}else{
		Uart_Print("+");
	}	
	Uart_SendChar('0'+ (_num / 100000) % 10);
	Uart_SendChar('0'+ (_num / 10000) % 10);
	Uart_SendChar('0'+ (_num / 1000) % 10);
	Uart_SendChar('0'+ (_num / 100) % 10);
	Uart_SendChar('0'+ (_num / 10) % 10);
	Uart_SendChar('0'+ _num % 10);	
	return;
}
