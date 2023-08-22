#include "init.h"

void InitIO(){
    // let D0, D1, D2 to be output, D6 to be input
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD6 = 1;
    

    // set pull up for RD6 (multiplexed with SW1 and CN15)
    CNPUEbits.CNPUE15 = 1;
}

void InitCLK(){
    SYSKEY = 0x0;               // Ensure OSCCON is lock
    SYSKEY = 0xAA996655;        // Unlock sequence part 1 of 2 back to back instructions.
    SYSKEY = 0x556699AA;        // Unlock sequence part 2 of 2 back to back instructions.
    OSCCONbits.NOSC = 0x0007;   // Write new osc src value to NOSC control bits -- FRS, with original freqeuncy as 8 MHz
    OSCCONbits.FRCDIV = 0x3; // the prescale of FRC is 8
    OSCCONbits.PBDIV = 0x0;    // PBCLK is SYSCLK divided by 1. {(Not changed here)Clock is multiplied by 15. PLL output is divided by 1} -- PBCLK has freqeuncy 1 MHz
    OSCCONbits.OSWEN = 0x0001;  // Initiate clock switch by setting OSWEN bit.
    SYSKEY = 0x0;               // Write non-key value to perform a re-lock.

    while(OSCCONbits.OSWEN);    // Loop until OSWEN = 0. Value of 0 indicates osc switch is complete.
}

void InitTimer(){
    // for Timer 1
    T1CON = 0x0;//prescale 1:1
    PR1 = 0xFFFF;//max period

    // for Timer 2
    T2CON = 0x0;//prescale 1:1
    PR2 = 20000;// period of 256
    
}

void StartTimer(){
    T1CONSET = 0x8000;//start timer 1
    T2CONSET = 0x8000;//start timer 2
}

void InitOC(){
    // for OC1
    OC1CON = 0x0;// first turn off oc1
    OC1R = 0x0000;// Prim CR as 0
    OC1RS = 0x0000;// Sec CR as 0
    OC1CON = 0x0006; // set oc1 to PWM mode without fault pin enabled 
}

void InitInterrupt(){
    /* Configure Timer interrupts */ 
    INTCONbits.MVEC = 1;        // multi-vector mode
    IPC1SET = 0x000d;           // timer 1: priority is 3, subpriority is 1
    // IPC2SET = 0x000d;           // timer 2: priority is 3, subpriority is 1
    IFS0CLR = 0x0110;           // clear the flags for timer 1 and timer 2

    /* enable global and individual interrupts */
    __asm( "ei" );                // enable interrupt globally by execute a assembly instruction "ei"
    IEC0SET = 0x0110;           // enable interrupt for timer 1 and timer 2
}

void InitMCU(){
    InitIO();
    InitCLK();
    InitTimer();
    InitOC();
    InitInterrupt();
    StartTimer();
}
