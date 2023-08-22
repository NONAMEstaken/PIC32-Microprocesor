# include "OC.h"

void EnablePWM1(){
    OC1CONSET = 0x8000;//start oc1
}
void DisablePWM1(){
    OC1CONCLR = 0x8000;//stop oc1
}

void SetPWM1R(uchar val){
    OC1R = val;
}

void SetPWM1RS(uchar val){
    OC1RS = val;
}
