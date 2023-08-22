#include "init.h"

void Init_IO(){
    // let D0, D1, D2 to be output, D6 to be input
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISGbits.TRISG1 = 0;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD13 = 1;
    TRISDbits.TRISD12 = 1;
    TRISFbits.TRISF0 = 1;
    TRISFbits.TRISF1 = 1;
    TRISDbits.TRISD10 = 0;  // SPI1 SCK
    TRISEbits.TRISE7 = 0;   // SPI1 D/C
    TRISBbits.TRISB0 = 1;   // ADC AN0
    TRISBbits.TRISB1 = 1;   // ADC AN1
    
    //RB3 output
    TRISBbits.TRISB3 = 0;

    // set pull up for RD6, RD7, RD13 (multiplexed with SW1 and CN15)
    CNPUEbits.CNPUE15 = 1;
    CNPUEbits.CNPUE16 = 1;
    CNPUEbits.CNPUE19 = 1;

}

void Init_CLK(){
    SYSKEY = 0x0;               // Ensure OSCCON is lock
    SYSKEY = 0xAA996655;        // Unlock sequence part 1 of 2 back to back instructions.
    SYSKEY = 0x556699AA;        // Unlock sequence part 2 of 2 back to back instructions.
    OSCCONbits.NOSC = 0x0007;   // Write new osc src value to NOSC control bits -- FRS, with original freqeuncy as 8 MHz
    OSCCONbits.FRCDIV = 0x0; // the prescale of FRC is 1
    OSCCONbits.PBDIV = 0x0;    // PBCLK is SYSCLK divided by 1. {(Not changed here)Clock is multiplied by 15. PLL output is divided by 1} -- PBCLK has freqeuncy 8 MHz
    OSCCONbits.OSWEN = 0x0001;  // Initiate clock switch by setting OSWEN bit.
    SYSKEY = 0x0;               // Write non-key value to perform a re-lock.

    while(OSCCONbits.OSWEN);    // Loop until OSWEN = 0. Value of 0 indicates osc switch is complete.
}

void Init_Timer(){
    // for Timer 1
    T1CON = 0x0;
    T1CONbits.TCKPS = 0b01; //prescale 1:8
    PR1 = 0xFFFF;//max period

    // for Timer 2
    T2CON = 0x0;
    T2CONbits.TCKPS = 0b011;//prescale 1:8
    PR2 = 20000;// freqeuncy of 50 Hz
    
}
/*
void Start_Timer(){
    T1CONSET = 0x8000;//start timer 1
    T2CONSET = 0x8000;//start timer 2
}
*/

void Init_OC(){
    // for OC3
    OC3CON = 0x0;// first turn off oc3
    OC3R = 0x0000;// Prim CR as 0
    OC3RS = 0x0000;// Sec CR as 0
    OC3CON = 0x0006; // set oc3 to PWM mode without fault pin enabled 
    OC3CONbits.OCTSEL = 0;  // use timer 2 as the clock source for OC3
    // for OC4
    OC4CON = 0x0;// first turn off oc4
    OC4R = 0x0000;// Prim CR as 0
    OC4RS = 0x0000;// Sec CR as 0
    OC4CON = 0x0006; // set oc4 to PWM mode without fault pin enabled 
    OC4CONbits.OCTSEL = 0;  // use timer 2 as the clock source for OC4
}

void Init_Interrupt(){
    INTCONbits.MVEC = 1;        // multi-vector mode
    /* Configure Timer interrupts */ 
    IPC1SET = 0x000d;           // timer 1: priority is 3, subpriority is 1
    // IPC2SET = 0x000d;           // timer 2: priority is 3, subpriority is 1
    IFS0CLR = 0x0110;           // clear the flags for timer 1 and timer 2
    
    // Configure UART interrupts
    IPC6SET = 0x0005;           // UART 1: priority is 1, subpriority is 1
    IPC8SET = 0x0006;           // UART 2: priority is 1, subpriority is 1
    IFS0bits.U1RXIF = 0;        // clear the flag for UART 1
    IFS1bits.U2RXIF = 0;        // clear the flag for UART 2

    /* enable global and individual interrupts */
    __asm( "ei" );                // enable interrupt globally by execute a assembly instruction "ei"
    // enable interrupt for timer 1, UART 1
    IEC0bits.T1IE = 1;
    //IEC0bits.T2IE = 1;
    IEC0bits.U1RXIE = 1;
    IEC1bits.U2RXIE = 1;

}

void Init_UART(){
    // for UART 1 (Radar)
    U1MODEbits.ON = 0;  // turn off UART1
    U1MODEbits.PDSEL = 1;   // 8 bit data, even parity
    U1MODEbits.STSEL = 0;   // 1 bit stop
    U1BRGSET = 51; // baud rate 38400: 8M/4*(51+1)
    U1MODEbits.ON = 1;  // turn on UART1
    U1STAbits.UTXEN = 1;// enable transmit
    U1STAbits.URXEN = 1;// enable receive
    U1STAbits.URXISEL = 0b00; // interrupt when receive buffer is not empty
    
    // for UART 2 (IPS screen)
    U2MODEbits.ON = 0;  // turn off UART2
    U2MODEbits.PDSEL = 0;   // 8 bit data, no parity
    U2MODEbits.STSEL = 0;   // 1 bit stop
    U2MODEbits.BRGH = 1;    // high speed mode
    U2BRGSET = 51; // baud rate 9600: 8M/16*(51+1)
    U2MODEbits.ON = 1;  // turn on UART2
    U2STAbits.UTXEN = 1;// enable transmit
    U2STAbits.URXEN = 1;// enable receive
    U2STAbits.URXISEL = 0b00; // interrupt when receive buffer is not empty
}

void Init_SPI(){
    uchar rData;
    //IEC0CLR = 0x03800000;

    SPI1CONbits.ON = 0;
    rData = SPI1BUF;
    SPI1STAT = 0x0;
    SPI1CON = 0x0;  
    SPI1CONbits.SRXISEL = 0;// MSB first
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.CKP = 0;    // 0: idle state is low, 1: idle state is high
    SPI1CONbits.CKE = 1;    // 1: data change from active to idle, 0: data change from idle to active
    //SPI1CONbits.ENHBUF = 1;
    SPI1BRGSET = 1;    // baud rate 2M: 8M/2*(1+1)

    SPI1CONbits.ON = 1;
}

void Init_ADC(){
    AD1CON1bits.ON = 1;
    AD1CON1bits.SSRC = 7;
    AD1CON3bits.SAMC = 2;
    AD1CHSbits.CH0SA = 0;
    AD1CON1bits.ASAM = 1;
    AD1CON2bits.VCFG = 0;

}

void Init_MCU(){
    Init_IO();
    Init_CLK();
    Init_Timer();
    Init_OC();
    Init_Interrupt();
    Init_UART();
    Init_SPI();
    Init_ADC();
}
