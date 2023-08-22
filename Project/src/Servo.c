#include "Servo.h"

void Enable_Servo1(){
    Enable_PWM1();
}

void Set_Servo1(uchar angle){
    Set_PWM1R(0);
    Set_PWM1RS((0.5 + ((float)angle/(float)90))*1000);
}
void Enable_Servo2(){
    Enable_PWM2();
}

void Set_Servo2(uchar angle){
    Set_PWM2R(0);
    Set_PWM2RS((0.5 + ((float)angle/(float)90))*1000);
}
