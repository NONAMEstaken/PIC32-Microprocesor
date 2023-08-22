#include "SPI.h"

void SPI1_Send_u8(uchar data){
    while(SPI1STATbits.SPITBF);
    SPI1BUF = data;
    while (SPI1STATbits.SPIBUSY);
}

void SPI1_Send_u16(uint data){
    // big endian transmission
    SPI1_Send_u8(data >> 8);
    SPI1_Send_u8((uchar)(data&0x00FF));
}

void SPI1_SendData(uchar *data, int len){
    int i = 0;
    for(i = 0; i < len; i++){
        SPI1_Send_u8(data[i]);
    }
}

void SPI1_Print(char *str){
    int len = strlen(str);
    SPI1_SendData((uchar *)str, len);
}
void SPI1_Print_uchar(uchar c){
    char str[4];
    uchar_2_str(c,str);
    int len = strlen(str);
    SPI1_SendData((uchar *)str, len);
}
void SPI1_Print_float(float c){
    char str[5];
    float_2_str(c,str);
    int len = strlen(str);
    SPI1_SendData((uchar *)str, len);
}
void SPI1_Print_uint(uint c){
    char str[6];
    uint_2_str(c,str);
    int len = strlen(str);
    SPI1_SendData((uchar *)str, len);
}

void SPI1_Print_Turrent_Para(Turret_para turrent_para){
    SPI1_Print("turret yaw:  ");
    SPI1_Print_float(turrent_para.yaw);
    SPI1_Print("    ");
    SPI1_Print("turret pitch:  ");
    SPI1_Print_float(turrent_para.pitch);
    SPI1_Print("\r\n");
}

void SPI1_Print_Vector3D(Vector3 vector){
    SPI1_Print("x:  ");
    SPI1_Print_float(vector.x);
    SPI1_Print("    ");
    SPI1_Print("y:  ");
    SPI1_Print_float(vector.y);
    SPI1_Print("    ");
    SPI1_Print("z:  ");
    SPI1_Print_float(vector.z);
    SPI1_Print("\r\n");
}

void SPI1_Print_RadarInfo(RadarInfo info){
    SPI1_Print("targetNum:  ");
    SPI1_Print_uchar(info.targetNum);
    SPI1_Print("\r\n");
    for (uchar i = 0; i < 10; i++) {
        if (info.targets[i].hasTarget) {
            SPI1_Print("  targetID:  ");
            SPI1_Print_uchar(i);
            SPI1_Print("\r\n    Distance:  ");
            SPI1_Print_uchar(info.targets[i].distance);
            SPI1_Print("\r\n    Yaw:  ");
            SPI1_Print_uchar(info.targets[i].yaw);
            SPI1_Print("\r\n    Pitch: ");
            SPI1_Print_uchar(info.targets[i].pitch);
            SPI1_Print("\r\n");
         }
    }
}
