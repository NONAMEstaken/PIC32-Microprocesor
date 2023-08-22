#ifndef SERVO_H
#define SERVO_H
#include "global.h"
#include "OC.h"

//use pwm 1 for servo 1
void Enable_Servo1();
void Set_Servo1(uchar angle);
//use pwm 2 for servo 2
void Enable_Servo2();
void Set_Servo2(uchar angle);

#endif // !SERVO_H
