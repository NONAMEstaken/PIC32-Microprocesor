#ifndef SPIHEADER
#define SPIHEADER

#include "global.h"
#include "Radar.h"

void SPI1_Send_u8(uchar data);
void SPI1_Send_u16(uint data);

void SPI1_SendData(unsigned char *data, int len);
void SPI1_Print(char *str);
void SPI1_Print_uchar(uchar c);
void SPI1_Print_float(float c);
void SPI1_Print_uint(uint c);

void SPI1_Print_Turrent_Para(Turret_para turrent_para);
void SPI1_Print_Vector3D(Vector3 vector);
void SPI1_Print_RadarInfo(RadarInfo info);

#endif // !SPIHEADER
