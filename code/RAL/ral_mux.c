//#include <lpc213x.h>
#include "ral_mux.h"
#include "../AAL/aal_gpio.h"

//global varible
//static int gpio_[5];

/*
 *	Mux_Init()
 *	init the mux
 *	Using 5 gpios to represent 4 select pin and 1 not_enable pine
 */
struct Mux_Struct* Mux_Init(struct Mux_Struct* Mux, int EN, int gpio[4]){ 
//Init GPIO[3] MS --> GPIO[0] LS
	int i;
	Mux->EN = EN;
	for(i=0;i<4;++i){
		MUX->gpio[i] = gpio[i];
		GPIO_Init(MUX->gpio[i], OUTPUT);
	}
	Mux_Unset(MUX);//deactive the mux(select no output)
	return MUX;
}

/*
 *	Mux_Set()
 *	low the selected pin
 */
void Mux_Set(struct Mux_Struct Mux, int channel){
	channel &= 0x0F;
	(channel&0x08) ? GPIO_Set(Mux->gpio[3],1) : GPIO_Set(Mux->gpio[3],0);
	(channel&0x04) ? GPIO_Set(Mux->gpio[2],1) : GPIO_Set(Mux->gpio[2],0);
	(channel&0x02) ? GPIO_Set(Mux->gpio[1],1) : GPIO_Set(Mux->gpio[1],0);
	(channel&0x01) ? GPIO_Set(Mux->gpio[0],1) : GPIO_Set(Mux->gpio[0],0);
	// active the mux	 		
	GPIO_Set(Mux->EN,0); //clear not_EN

	////example about what above code doing
	//switch(channel){
	//	case MUX_2103:
	//		// select the SS1
	//		IO0CLR |= 1<<16; //clear p0.16
	//		IO0CLR |= 1<<17; //clear p0.17
	//		IO0CLR |= 1<<18; //clear p0.18
	//		IO0CLR |= 1<<19; //clear p0.19
	//		// active the mux	 		
	//		IO0CLR |= 1<<20; //clear p0.20
	//		break;
	//	case MUX_ENCODER1:
	//		// select the SS2
	//		IO0CLR |= 1<<16; //clear p0.16
	//		IO0CLR |= 1<<17; //clear p0.17
	//		IO0SET |= 1<<18; //set p0.18
	//		IO0CLR |= 1<<19; //clear p0.19
	//		// active the mux	 		
	//		IO0CLR |= 1<<20; //clear p0.20
	//		break;
	//}

}

/*
 *	Mux_Unset()
 *	deactive the mux(select no output), high all pin
 * */
void Mux_Unset(struct MUX_Struct Mux){
	// deactive the mux	 		
	GPIO_Set(Mux->EN,1); //set not_EN
	return ;
}

