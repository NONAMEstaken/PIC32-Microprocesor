# include "OC.h"

void Enable_PWM1(){
    T2CONSET = 0x8000;//start timer 2
    OC3CONSET = 0x8000;//start oc1
}
void Disable_PWM1(){
    T2CONCLR = 0x8000;//stop timer 2
    OC3CONCLR = 0x8000;//stop oc1
}

void Set_PWM1R(uint val){
    OC3R = val;
}

void Set_PWM1RS(uint val){
    OC3RS = val;
}


void Enable_PWM2(){
    T2CONSET = 0x8000;//start timer 2
    OC4CONSET = 0x8000;//start oc1
}
void Disable_PWM2(){
    T2CONCLR = 0x8000;//stop timer 2
    OC4CONCLR = 0x8000;//stop oc1
}

void Set_PWM2R(uint val){
    OC4R = val;
}

void Set_PWM2RS(uint val){
    OC4RS = val;
}
