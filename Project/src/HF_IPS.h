#ifndef HF_IPS_H
#define HF_IPS_H

#include "global.h"
#include "Radar.h"
#include "UART.h"

typedef struct {
    bool hasTarget;
    u_Vector2 pos;
} TargetSpot;

#define IPS_BLACK 0
#define IPS_GREEN 29
#define IPS_YELLOW 28


extern uchar ScreenOK_Buffer[5];
extern uchar ScreenOK_Index;
extern TargetSpot TargetSpot_Buffer[10];
extern TargetSpot TDCSpot_Buffer;


void Draw_RadarTarget(TargetInfo info);

void IPS_RESET();
void IPS_CMD_EXECUTE();
void IPS_BOXF(uint x, uint y, uint w, uint h, uint color);
void IPS_CIR(uint x, uint y, uint r, uint color);
void IPS_LINE(uint xs, uint ys, uint xe, uint ye, uint color);
void IPS_DRAW_TARGET(TargetInfo info, float zoom);
void IPS_CLR_TARGET(u_Vector2 info);
bool IPS_CLR_ALL_TARGET();
void IPS_DRAW_TDC(u_Vector2 pos, float zoom);
bool IPS_CLR_TDC();

void IPS_CHECKBUSY();

#endif // !HF_IPS_H

