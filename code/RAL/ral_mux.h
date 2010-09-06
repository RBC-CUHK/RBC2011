#ifndef __ral_mux_h
#define __ral_mux_h

#define MUX_2103 0
#define MUX_ENCODER1 2
#define MUX_ENCODER2 3
#define MUX_ENCODER3 4
#define MUX_ENCODER4 5
#define MUX_ENCODER5 6
#define MUX_ENCODER6 7
#define MUX_ENCODER7 8
#define MUX_ENCODER8 9
#define MUX_ENCODER9 10
#define MUX_ENCODER10 11

struct Mux_Struct{
	int EN;
	int gpio[4];
};

/*
 * 5 pins, not_EN for enable of mux, other pins for selecting output
 */
struct Mux_Struct* Mux_Init(struct Mux_Struct* Mux, int EN, int gpio[4]); //Init GPIO[3] MS --> GPIO[0] LS
void Mux_Set(struct Mux_Struct Mux, int channel);
void Mux_Unset(struct Mux_Struct Mux);

#endif

