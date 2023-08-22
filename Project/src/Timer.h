#ifndef TIMER
#define TIMER

#include "global.h"

typedef struct {
    unsigned timer1_done : 1;
}bits; 

extern bits Timer1Flags;
// use Timer 1
void Timer1_ON(short int num);// use with great care
void Timer1_OFF();// use with great care
void DelayUsec(short int num);
void DelayMsec(int num);

#endif // !TIMER

