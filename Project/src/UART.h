#ifndef UART
#define UART

#include "global.h"
#include "Radar.h"

void UART1_SendData(unsigned char *data, int len);
void UART2_SendData(unsigned char *data, int len);

void U2_Print(char *str);
void U2_Print_uchar(uchar c);
void U2_Print_float(float c);
void U2_Print_uint(uint c);


// TODO: move to SPI
void U2_Print_RadarInfo(RadarInfo info);
void U2_Print_Turrent_Para(Turret_para turrent_para);
void U2_Print_Vector3D(Vector3 vector);



#endif // !UART
