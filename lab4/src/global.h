#ifndef GLOBAL
#define GLOBAL

#include <pic32m-libs/p32xxxx.h>

#define Button PORTDbits.RD6
#define LED LATDbits.LATD0
#define LED2 LATDbits.LATD1
#define LED3 LATDbits.LATD2

typedef unsigned char uchar;

#endif // !GLOBAL
