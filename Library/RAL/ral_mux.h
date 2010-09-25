/**
 *	@file
 *	@brief	RAL_Mux Header File
 * */
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

/**
 *	@brief	Store the five pins of a MUX
 * */
struct Mux_Struct{
	int EN;			///< Not_Enable Pin, GPIO Standard
	int gpio[4];	///< GPIO pins for the mux select	MS->LS, GPIO Standard
};

struct Mux_Struct* Mux_Init(struct Mux_Struct* Mux, int EN, int gpio[4]);
void Mux_Set(struct Mux_Struct* Mux, int channel);
void Mux_Unset(struct Mux_Struct* Mux);

#endif

