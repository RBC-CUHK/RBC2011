#ifndef __ral_encoder_h
#define __ral_encoder_h 
#include "ral_mux.h"

void Encoder_Init(struct Mux_Struct* Mux);
int Encoder_Read(int channel);
int Encoder_ReadBuffer(int channel);
void Encoder_ReadAll(void);
void Encoder_Reset(int channel);

#endif

