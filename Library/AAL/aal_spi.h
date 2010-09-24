#ifndef __aal_spi_h
#define __aal_spi_h

void SPI_InitMaster(int clkDivider);//default using 8 bit sending mode
void SPI_InitSlave(int clkDivider, void (*callback)(void) __irq);
void SPI_SetLength(int length);//can only work at 8 or 16 bit because of hard coding
int SPI_Send(int data);
int SPI_Recieve(void);

#endif

