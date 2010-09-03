//#include <lpc213x.h>
#include "ral_mux.h"
#include "../AAL/aal_gpio.h"

//global varible
static int gpio_[5];

/*
 *	Mux_Init()
 *	init the mux
 *	Using 5 gpios to represent 4 select pin and 1 not_enable pine
 */
void Mux_Init(int gpio[5]){ //Init GPIO ##GPIO[4] not_EN, GPIO[3] MS --> GPIO[0] LS
	int i;
	for(i=0;i<5;++i){
		gpio_[i] = gpio[i];
		GPIO_Init(gpio_[i], OUTPUT);
	}
	Mux_Unset();//deactive the mux(select no output)
}

/*
 *	Mux_Set()
 *	low the selected pin
 */
void Mux_Set(int channel){
	channel &= 0x0F;
	(channel&0x08) ? GPIO_Set(gpio_[3],1) : GPIO_Set(gpio_[3],0);
	(channel&0x04) ? GPIO_Set(gpio_[2],1) : GPIO_Set(gpio_[2],0);
	(channel&0x02) ? GPIO_Set(gpio_[1],1) : GPIO_Set(gpio_[1],0);
	(channel&0x01) ? GPIO_Set(gpio_[0],1) : GPIO_Set(gpio_[0],0);
	// active the mux	 		
	GPIO_Set(gpio_[4],0); //clear not_EN

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
void Mux_Unset(void){
	// deactive the mux	 		
	GPIO_Set(gpio_[4],1); //set not_EN
}

