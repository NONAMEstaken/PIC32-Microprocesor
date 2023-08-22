#include "HF_IPS.h"
#include "Timer.h"
#include "SPI.h"

uchar ScreenOK_Buffer[5];
uchar ScreenOK_Index = 0;
TargetSpot TargetSpot_Buffer[10];
TargetSpot TDCSpot_Buffer;

void IPS_RESET(){
    U2_Print("\r\n");
    DelayMsec(500);
    U2_Print("RESET();\r\n");
    DelayMsec(1200);
}

void IPS_CMD_EXECUTE(){
    IPS_CHECKBUSY();
    U2_Print("\r\n");
    ScreenExcution_OK = false;
    ScreenOK_Index = 0;
}

void IPS_BOXF(uint x, uint y, uint w, uint h, uint color){
    U2_Print("BOXF(");
    U2_Print_uint(x-w/2);
    U2_Print(",");
    U2_Print_uint(y-h/2);
    U2_Print(",");
    U2_Print_uint(x+w/2);
    U2_Print(",");
    U2_Print_uint(y+h/2);
    U2_Print(",");
    U2_Print_uint(color);
    U2_Print(");");
}

void IPS_CIR(uint x, uint y, uint r, uint color){
    U2_Print("CIR(");
    U2_Print_uint(x);
    U2_Print(",");
    U2_Print_uint(y);
    U2_Print(",");
    U2_Print_uint(r);
    U2_Print(",");
    U2_Print_uint(color);
    U2_Print(");");
}

void IPS_LINE(uint xs, uint ys, uint xe, uint ye, uint color){
    U2_Print("PL(");
    U2_Print_uint(xs);
    U2_Print(",");
    U2_Print_uint(ys);
    U2_Print(",");
    U2_Print_uint(xe);
    U2_Print(",");
    U2_Print_uint(ye);
    U2_Print(",");
    U2_Print_uint(color);
    U2_Print(");");
}

void IPS_DRAW_TARGET(TargetInfo info, float zoom){
    uint x = (info.yaw-30) * 1.75f; // 30-150 : 0-210 : 15-225
    uint y = (info.distance) * 2.875f * zoom; // 0-80 : 0-230 : 245-15
    // restruct x and y
    if (x < 0) x = 0;
    if (x > 210) x = 210;
    if (y < 0) y = 0;
    if (y > 230) y = 230;

    IPS_BOXF(x+15, 245-y, 9, 5, 29);

    for (int i = 0; i<10; i++) {
        if (!TargetSpot_Buffer[i].hasTarget) {
            TargetSpot_Buffer[i].hasTarget = true;
            TargetSpot_Buffer[i].pos.x = x;
            TargetSpot_Buffer[i].pos.y = y;
            break;
        }
    }
}

void IPS_CLR_TARGET(u_Vector2 info){
    uint x = info.x; // 30-150 : 0-210 : 15-225
    uint y = info.y; // 0-80 : 0-230 : 245-15
    // restruct x and y
    if (x < 0) x = 0;
    if (x > 209) x = 209;
    if (y < 1) y = 1;
    if (y > 230) y = 230;
    IPS_BOXF(x+15, 245-y, 9, 6, 0);

    // judge whether intersection with line happen
    uint xs = x+15-10/2;
    uint ys = 245-y-6/2;
    uint xe = x+15+10/2;
    uint ye = 245-y+6/2;
    if(xs<=65 && xe>=65){
        IPS_LINE(65, ys, 65, ye, IPS_GREEN);
    }else if (xs<=120 && xe>=120){
        IPS_LINE(120, ys, 120, ye, IPS_GREEN);    
    }else if (xs<=175 && xe>=175) {
        IPS_LINE(175, ys, 175, ye, IPS_GREEN);
    }

    if (ys<=70 && ye>=70) {
        IPS_LINE(xs, 70, xe, 70, IPS_GREEN);
    }else if (ys<=130 && ye>=130) {
        IPS_LINE(xs, 130, xe, 130, IPS_GREEN);
    }else if (ys<=190 && ye>=190) {
        IPS_LINE(xs, 190, xe, 190, IPS_GREEN);
    }


}

bool IPS_CLR_ALL_TARGET(){
    bool hasCommand = false;
    for (int i = 0; i<10; i++) {
        if (TargetSpot_Buffer[i].hasTarget) {
            IPS_CLR_TARGET(TargetSpot_Buffer[i].pos);
            TargetSpot_Buffer[i].hasTarget = false;
            hasCommand = true;
        }
    }
    return hasCommand;
}


void IPS_DRAW_TDC(u_Vector2 info, float zoom){
    uint x = info.x; // 30-150 : 0-210 : 15-225
    uint y = info.y * zoom; // 0-80 : 0-230 : 245-15
    // restruct x and y
    if (x < 0) x = 0;
    if (x > 200) x = 200;
    if (y < 0) y = 0;
    if (y > 220) y = 220;

    IPS_BOXF(x+20-8, 240-y, 3, 18, IPS_YELLOW);
    IPS_BOXF(x+20+8, 240-y, 3, 18, IPS_YELLOW);

    TDCSpot_Buffer.hasTarget = true;
    TDCSpot_Buffer.pos.x = x;
    TDCSpot_Buffer.pos.y = y;
}

bool IPS_CLR_TDC(){
    if (!TDCSpot_Buffer.hasTarget) {
        return false;
    }
    uint x = TDCSpot_Buffer.pos.x; // 30-150 : 0-210 : 15-225
    uint y = TDCSpot_Buffer.pos.y; // 0-80 : 0-230 : 245-15
    // restruct x and y
    if (x < 0) x = 0;
    if (x > 199) x = 199;
    if (y < 1) y = 1;
    if (y > 220) y = 220;
    IPS_BOXF(x+20, 240-y, 19, 19, IPS_BLACK);

    // judge whether intersection with line happen
    uint xs = x+20-19/2;
    uint ys = 240-y-19/2;
    uint xe = x+20+19/2;
    uint ye = 240-y+19/2;
    if(xs<=65 && xe>=65){
        IPS_LINE(65, ys, 65, ye, IPS_GREEN);
    }else if (xs<=120 && xe>=120){
        IPS_LINE(120, ys, 120, ye, IPS_GREEN);    
    }else if (xs<=175 && xe>=175) {
        IPS_LINE(175, ys, 175, ye, IPS_GREEN);
    }

    if (ys<=70 && ye>=70) {
        IPS_LINE(xs, 70, xe, 70, IPS_GREEN);
    }else if (ys<=130 && ye>=130) {
        IPS_LINE(xs, 130, xe, 130, IPS_GREEN);
    }else if (ys<=190 && ye>=190) {
        IPS_LINE(xs, 190, xe, 190, IPS_GREEN);
    }
    return true;
}




void IPS_CHECKBUSY(){
    long int timeout = 0;
    while (!ScreenExcution_OK && timeout<20000){
        SPI1_Print_uint(timeout);
        timeout++;
    }
}


//ISF for UART2 RX
#pragma interrupt U2_RX_Interrupt ipl1 vector 32
void U2_RX_Interrupt(void){
    if (IFS1bits.U2RXIF == 1) {
        // read the data
        uchar data;
        while (U2STAbits.URXDA) {
            data = U2RXREG;
            ScreenOK_Buffer[ScreenOK_Index] = data;
            ScreenOK_Index ++;
            if(data == 0x4F){
                //SPI1_Print((char *)ScreenOK_Buffer);
                // update radar info using data in buffer
                ScreenExcution_OK = true;
                // reset buffer
                ScreenOK_Index = 0;
            }

        }
        IFS1bits.U2RXIF = 0; // clear the interrupt flag
    }else{
        // not a U2 RX interrupt
        return;
    }
}
