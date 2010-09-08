#include "ral_encoder.h"
#include "ral_mux.h"
#include "../AAL/aal_spi.h"

//global varibles: local buffer of all encoder values
static int encoder_value_[20] ={0};
static struct Mux_Struct* EncoderMux;

/*
 *	Encoder_Init()
 *	init spi and mux
 *	reset all encoder values to zero
 * */
void Encoder_Init(struct Mux_Struct* Mux){
	/*
	 * array of pin for mux
	 * p0.20 is not_EN
	 * p0.16-p0.19 is selecting bits
	 */
//	int gpio[4]={18,17,16,20};
//	int EN = 19;
	int i;

//	Mux_Init(EncoderMux,EN,gpio); //Init GPIO ##GPIO[4] EN, GPIO[3] MS --> GPIO[0] LS
	EncoderMux = Mux;
	SPI_InitMaster(16);
	for(i=MUX_ENCODER1 ; i<=MUX_ENCODER10 ;i++)
	{
		Encoder_Reset(i);
	}
}

/*
 *	Encoder_Reset()
 *	reset all value of encoder chip LS7366
 * */
void Encoder_Reset(int channel){
	int i;
	int data;

	SPI_setLength(8);//encoder use 32-bit mode, we use 4 times 8 bits sending to make it work

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

/*
 *	Encoder_Read()
 *	Read the encoder value from one channel
 * */
int Encoder_Read(int channel){
	int i,k;
	int data;

	SPI_setLength(8);//encoder use 32-bit mode, we use 4 times 8 bits sending to make it work
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
/*
 *	Encoder_ReadAll()
 *	Store all encoder values in a local buffer for reading
 * */
void Encoder_ReadAll(){
	int i;
	for(i=MUX_ENCODER1 ; i<=MUX_ENCODER10 ;i++)
	{
		encoder_value_[i] = Encoder_Read(i);
	}
}
/*
 *	Encoder_ReadBuffer()
 *	Store all encoder values in a local buffer for reading
 * */
int Encoder_ReadBuffer(int channel){
	return encoder_value_[channel];
}
