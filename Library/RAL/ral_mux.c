/**
 *	@file
 *	@brief	RAL_Mux Function Implementation
 * */
#include "ral_mux.h"
#include "../AAL/aal_gpio.h"

/**
 *	@see	Mux_Struct
 *	@brief	Init the Mux
 *
 *	Would accordingly init the GPIOs to Output pins.
 *	
 *	@param	Mux		Mux to be inited
 *	@param	EN		Not Enable pin
 *	@param	gpio	Select pins
 *
 *	@return	Pointer to the inited Mux_Struct
 */
struct Mux_Struct* Mux_Init(struct Mux_Struct* Mux, int EN, int gpio[4]){ 
	int i;
	Mux->EN = EN;
	GPIO_Init(EN,OUTPUT);
	for(i=0;i<4;i++){
		Mux->gpio[i] = gpio[i];
		GPIO_Init(gpio[i], OUTPUT);
	}
	Mux_Unset(Mux);//deactive the mux(select no output)
	return Mux;
}

/**
 *	@brief	Set the corresponding channel of the mux
 *
 *	Would activate the Mux
 *
 *	@param	Mux		Mux to be set
 *	@param	channel	channel to be set
 * */
void Mux_Set(struct Mux_Struct* Mux, int channel){
	channel &= 0x0F;
	(channel&0x08) ? GPIO_Set(Mux->gpio[0],1) : GPIO_Set(Mux->gpio[0],0);
	(channel&0x04) ? GPIO_Set(Mux->gpio[1],1) : GPIO_Set(Mux->gpio[1],0);
	(channel&0x02) ? GPIO_Set(Mux->gpio[2],1) : GPIO_Set(Mux->gpio[2],0);
	(channel&0x01) ? GPIO_Set(Mux->gpio[3],1) : GPIO_Set(Mux->gpio[3],0);
	// active the mux	 		
	GPIO_Set(Mux->EN,0); //clear not_EN
}

/**
 *	@brief	Deactive the mux
 *	
 *	@param	Mux		Mux to be deactived
 * */
void Mux_Unset(struct Mux_Struct* Mux){
	// deactive the mux	 		
	GPIO_Set(Mux->EN,1); //set not_EN
	return ;
}

