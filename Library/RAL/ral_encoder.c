/**
 *	@file
 *	@brief	RAL_Encoder Function Implementation
 */
#include "ral_encoder.h"
#include "ral_mux.h"
#include "AAL/aal_spi.h"

//global varibles: local buffer of all encoder values
static int encoder_value_[20] ={0};

//the Mux use in Encoder
static struct Mux_Struct* EncoderMux;

/**
 *	Init Encoders, would also init SPI and mux
 *	reset all encoder values to zero
 *	@param Mux	Mux used for Encoders
 */
void Encoder_Init(struct Mux_Struct* Mux){
	/*
	 * array of pin for mux
	 * p0.20 is not_EN
	 * p0.16-p0.19 is selecting bits
	 */
	int i;

//	Mux_Init(EncoderMux,EN,gpio); //Init GPIO ##GPIO[4] EN, GPIO[3] MS --> GPIO[0] LS
	EncoderMux = Mux;
	SPI_InitMaster(16);
	for(i=MUX_ENCODER1 ; i<=MUX_ENCODER10 ;i++)
	{
		Encoder_Reset(i);
	}
}

/**
 *	Reset the specified encoder
 *	@param channel	The encoder to be reset
 */
void Encoder_Reset(int channel){
	int i;
	int data;

	SPI_SetLength(8);//encoder use 32-bit mode, we use 4 times 8 bits sending to make it work

	/*
	 *set the MDR0
	 */
	Mux_Set(EncoderMux,channel);// select channel (low that pin)

	data = 0;
	data |= (1<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<3);//;LS7366 WR_MDR0 command

	SPI_Send(data);

	data = 0;
	data |= (0<<7) |//Filter clock division factor = 1
		(0<<6) |//Asynchronous Index
		(0<<5) |//disable index
		(0<<4) |//disable index
		(0<<3) |//free-running count mode
		(0<<2) |//free-running count mode
		(1<<1) |//x4 quadrature count mode
		(1<<0) ;//x4 quadrature count mode

	SPI_Send(data);

	Mux_Unset(EncoderMux);// select no pin(high all pin)

	for(i=0;i<100;i++){}//delay

	/*
	 *  set the MDR1
	 */
	Mux_Set(EncoderMux,channel);// select channel (low that pin)

	data = 0;
	data |= (0<<7) | (0<<6) | (0<<5) | (1<<4) | (0<<3);//;LS7366 CLR_MDR1 command

	SPI_Send(data);

	Mux_Unset(EncoderMux);// select no pin(high all pin)

	for(i=0;i<100;i++){}//delay

	/*
	 *  set the CNTR
	 */
	Mux_Set(EncoderMux,channel);// select channel (low that pin)

	data = 0;
	data |= (0<<7) | (0<<6) | (1<<5) | (0<<4) | (0<<3);//;LS7366 CLR_CNTR command

	SPI_Send(data);

	Mux_Unset(EncoderMux);// select no pin(high all pin)

	for(i=0;i<100;i++){}//delay
}

/**
 *	Read the encoder value from one channel
 *	@param channel	The encoder to be reset
 *	@return Rhe encoder value
 */
int Encoder_Read(int channel){
	int i,k;
	int data;

	SPI_SetLength(8);//encoder use 32-bit mode, we use 4 times 8 bits sending to make it work
	/*
	 *  get value from LS7366R
	 */
	Mux_Set(EncoderMux,channel);// select channel (low that pin)

	data = 0;
	data |= (0<<7) | (1<<6) | (1<<5) | (0<<4) | (0<<3);//;LS7366 RD_CNTR command

	SPI_Send(data);

 	data=0;
	for(k=0;k<4;k++){
		for (i=0;i<100;i++){}//delay
		i=SPI_Send(1);//dummy send, receive data from LS7366;
		data = (data<<8) + i;
	}

	Mux_Unset(EncoderMux);// select no pin(high all pin)
	return data;
}

/**
 *	Read and Store all encoder values in a local buffer
 */
void Encoder_ReadAll(){
	int i;
	for(i=MUX_ENCODER1 ; i<=MUX_ENCODER10 ;i++)
	{
		encoder_value_[i] = Encoder_Read(i);
	}
}
/**
 *	Read out the buffered encoder value of specified channel
 *	@param	channel	The encoder to be reset
 *	@return	Buffered encoder value
 */
int Encoder_ReadBuffer(int channel){
	return encoder_value_[channel];
}
