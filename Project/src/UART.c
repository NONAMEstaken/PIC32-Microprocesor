#include "UART.h"

// some standard command message


// send data using UART1
void UART1_SendData(uchar *data, int len){
    int i = 0;
    for(i = 0; i < len; i++){
        while(U1STAbits.UTXBF);// wait until the transmit buffer is not full
        U1TXREG = data[i];
    }
}
void UART2_SendData(uchar *data, int len){
    int i = 0;
    for(i = 0; i < len; i++){
        while(U2STAbits.UTXBF);// wait until the transmit buffer is not full
        U2TXREG = data[i];
    }
}

void U2_Print(char *str){
    int len = strlen(str);
    UART2_SendData((uchar *)str, len);
}
void U2_Print_uchar(uchar c){
    char str[4];
    uchar_2_str(c,str);
    int len = strlen(str);
    UART2_SendData((uchar *)str, len);
}
void U2_Print_float(float c){
    char str[5];
    float_2_str(c,str);
    int len = strlen(str);
    UART2_SendData((uchar *)str, len);
}
void U2_Print_uint(uint c){
    char str[6];
    uint_2_str(c,str);
    int len = strlen(str);
    UART2_SendData((uchar *)str, len);
}

void U2_Print_Turrent_Para(Turret_para turrent_para){
    U2_Print("turret yaw:  ");
    U2_Print_float(turrent_para.yaw);
    U2_Print("    ");
    U2_Print("turret pitch:  ");
    U2_Print_float(turrent_para.pitch);
    U2_Print("\r\n");
}

void U2_Print_Vector3D(Vector3 vector){
    U2_Print("x:  ");
    U2_Print_float(vector.x);
    U2_Print("    ");
    U2_Print("y:  ");
    U2_Print_float(vector.y);
    U2_Print("    ");
    U2_Print("z:  ");
    U2_Print_float(vector.z);
    U2_Print("\r\n");
}

void U2_Print_RadarInfo(RadarInfo info){
    U2_Print("targetNum:  ");
    U2_Print_uchar(info.targetNum);
    U2_Print("\r\n");
    for (uchar i = 0; i < 10; i++) {
        if (info.targets[i].hasTarget) {
            U2_Print("  targetID:  ");
            U2_Print_uchar(i);
            U2_Print("\r\n    Distance:  ");
            U2_Print_uchar(info.targets[i].distance);
            U2_Print("\r\n    Yaw:  ");
            U2_Print_uchar(info.targets[i].yaw);
            U2_Print("\r\n    Pitch: ");
            U2_Print_uchar(info.targets[i].pitch);
            U2_Print("\r\n");
        }
    }
}

