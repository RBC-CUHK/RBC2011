/**
 *	@file
 *	@brief	AAL_SPI Function Implementation
 * */
#include "armversion.h"
#include "aal_spi.h"
#include "RAL/ral_mux.h"

/**
 *	@brief	Init the SPI in Master Mode
 *	used when running at Master mode
 *	enable SPI0, default sending at 8 bit mode
 *	MSB transfer first, interrupt inhibit
 *
 *	@param	clkDivider	Clock Divider
 * */
void SPI_InitMaster(int clkDivider){

	PINSEL0 |= 1<<10;       //p0.5 Pin sel for MISO0
	PINSEL0 |= 1<<12;       //p0.6 Pin sel for MOSI0
	PINSEL0 |= 1<<8;        //p0.4 Pin sel for SCK0

	S0SPCCR = clkDivider;        //Clock counter reg, spi clk = PCLK/speed_divider
	SPI_SetLength(8);		//default sending 8 bit mode
}

/**
 *	@brief	Init the SPI in Slave Mode
 *	used when running at Master mode
 * 	enable SPI0, 16 bit mode
 *	MSB transfer first, using interrupt enable
 *
 *	@param	clkDivider	Clock Divider
 *	@param	callback	Call Back function when SPI received
 */
void SPI_InitSlave(int clkDivider, void (*callback)(void) __irq){
	VICVectAddr1 = (unsigned long)callback;
	VICVectCntl1 = 0x2A; // Channel0 on Source#10 ... enabled
	VICIntEnable |= 0x400; // 10th bit is the SPI0 interrupt

	PINSEL0 |= 1<<10;       //p0.5 Pin sel for MISO0
	PINSEL0 |= 1<<12;       //p0.6 Pin sel for MOSI0
	PINSEL0 |= 1<<8;        //p0.4 Pin sel for SCK0
	PINSEL0 |= 1<<14;       //p0.7 Pin sel for SSEL0

	S0SPCR=	(0<<3) | //CPHA=0
		(0<<4) | //CPOL=0
		(0<<5) | //Slave mode
		(1<<7) | //SPIE=1 
		(1<<2) | //16 bit 
		(0<<8) | (0<<9) | (0<<10) | (0<11);//16 bit
	//control reg, phase=0, polarity=0,
	//slave mode, 16bits, MSB transfer first, interrupt enable
}

/**
 *	@brief	Set the SPI transmit length
 *	used when running at Master mode
 *	can only work at 8 or 16 bit because of hard coding
 *	@param	length	SPI transmit length
 */
void SPI_SetLength(int length){
	if(length == 8){
		S0SPCR = (0<<3) | //CPHA=0
			(0<<4)	| //CPOL=0
			(1<<5)  | //master mode
			(0<<6)	;//MSB (bit 7) first  
		//control reg, phase=0, polarity=0,
		//master mode, 8bits, MSB transfer first, interrupt inhibit
	}
	else if(length == 16){
		S0SPCR=	(1<<2) | //16 bit
			(0<<3) | //CPHA=0
			(0<<4) | //CPOL=0
			(1<<5) | //master mode
			(0<<8) | (0<<9) | (0<<10) | (0<11);//16 bit
		//control reg, phase=0, polarity=0,
		//master mode, 16bits, MSB transfer first, interrupt inhibit
	}
	else{
		//error
//		Uart_Print("SPI_setLength");
//		Uart_Print("(): wrong length, only 8 or 16 is allowed");
//		Uart_Print("\r\n");
//		exit(0);
		return ;
	}
}

/**
 *	@brief Send(and receive) data through SPI
 *
 *	becare that it needs to have a slave slect(SS) pin selecting process 
 *	before and after SPI_Send()
 *
 *	@param	data	data to be sent
 *	@return	received data
 * */ 
int SPI_Send(int data){
	int retval = 0;
	S0SPDR = data; 
	while((S0SPSR&0x80)==0){
	}       //wait until the trasnfer(send) is completed
	retval = S0SPDR;// get data
	return retval;
}
/**
 *	@brief	Receive data through SPI
 *
 *	receive data through SPI
 *	becare that it needs to have a slave slect(SS) pin selecting process 
 *	before and after SPI_Recieve()
 *	it just send dummy 1 and get data
 *	Recommand just using SPI_Send as they are the same function
 *
 *	@return	received data
 *
 *	@see	SPI_Send
 * */
int SPI_Recieve(void){
	return SPI_Send(1);
}


/*
 *	Example:
 *	This is an example of the callback function 
 *	when using slave interrupt mode
 *	Remeber to use __irq to decare function
 *	It needs to global variables: int spi_get_data, int spi_send_data
 *
 * */

//void __irq IRQ_Eint()
//{
//	if ((S0SPSR & 0xF8) == 0x80){
//		input = S0SPDR; // read byte from master, clear the flag
//	}
//	spi_get_data = input;
//	//work here we you want to set something just after receiving
//	//data, or you can use spi_get_data in main to get data
//	
//	//send the data out using spi_send_data
//	S0SPDR = spi_send_data;
//
//	S0SPINT = 0x01; // reset interrupt flag
//	VICVectAddr = 0; // reset VIC
//}
