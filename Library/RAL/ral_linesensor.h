/**
 *	@file
 *	@brief	RAL_Linesensor Header File
 * */

/**
 * 	Output Pins (12)	Input Pins (12)
 *	 xx  11  31  xx 	 xx  00  07  xx 
 *	 12  32  13  21 	 11  01  06  10  
 *	 22  14  23  33  	 08  02  05  09 
 *	 xx  24  34  xx  	 xx  03  04  xx 
 * */
void Linesensor_Init(int InputPins[12], int OutputPins[12]);

void Linesensor_Update(void);

int Linesensor_Read(int channel);

int Linesensor_ReadAll(void);

int Linesensor_ReadAllTrans(void);
