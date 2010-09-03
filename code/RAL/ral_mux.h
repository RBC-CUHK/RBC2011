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

/*
 * 5 pins, not_EN for enable of mux, other pins for selecting output
 */
void Mux_Init(int gpio[5]); //Init GPIO ##GPIO[4] EN, GPIO[3] MS --> GPIO[0] LS
void Mux_Set(int channel);
void Mux_Unset(void);

#endif

