#include "global.h"

bool RadarInfo_Updated = false;
bool ScreenExcution_OK = true;

char * uchar_2_str(uchar c, char * str){
    sprintf(str, "%u", c);
    return str;
}
char * float_2_str(float c, char * str){
    sprintf(str, "%.4g", c);
    return str;
}
char * uint_2_str(uint c, char * str){
    sprintf(str, "%u", c);
    return str;
}

float float_clamp(float data, float min, float max){
    if (data<min) {
        return min;
    }else if (data>max) {
        return max;
    }else{
        return data; 
    }
}

