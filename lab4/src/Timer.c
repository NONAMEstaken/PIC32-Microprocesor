#include "Timer.h"

//define a interrupt flag struct
struct bits {
    unsigned timer1_done : 1;
//    ...other user defined flags may go here
} Timer1Flags;


/* configure timer SFRs to generate num us delay*/
void DelayUsec(short int num) {
    // ...your code goes here
    PR1 = num;
    Timer1Flags.timer1_done = 0;
    T1CONSET = 0x8000;              // turn on the timer 1
    TMR1 = 0x0;                     // reset the timer 1
    while ( ! Timer1Flags.timer1_done );  // loop until flag 04 (for timer 1) is set
    Timer1Flags.timer1_done = 0;          // reset the flags
    T1CONCLR = 0x8000;                    // turn off timer 
}

/* configure timer SFRs to generate 1 ms delay*/
void GenMsec() {
    DelayUsec(1000);
}

/* Call GenMsec() num times to generate num ms delay*/
void DelayMsec(int num) {
    int i;
    for (i=0; i<num; i++) {
        GenMsec();
    }
}

/* timer 1 interrupt handler */ 
#pragma interrupt timer_1_interrupt ipl3 vector 4
void timer_1_interrupt(void) {
    T1CONCLR = 0x8000;                // turn off the timer 1
    IFS0CLR = 0x0010;           // clear the flag for timer 1
    Timer1Flags.timer1_done = 1;      // set up the flags
    TMR1 = 0;
}
