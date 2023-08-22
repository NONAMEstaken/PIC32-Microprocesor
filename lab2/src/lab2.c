#include <pic32m-libs/p32xxxx.h>

# define DELAY 20000

int delay_half_sec(int number){
    int i;
    for (i = 0; i < number*DELAY; i++);
}
int main(){
    DDPCONbits.JTAGEN = 0;
    TRISD = 0xfff0;
    PORTD = 0;
    while (1) {
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 1;
        delay_half_sec(1);
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 0;
        delay_half_sec(1);
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 1;
        delay_half_sec(1);
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 0;
        delay_half_sec(1);
        if (PORTDbits.RD6==0) {
            delay_half_sec(3);
                if (PORTDbits.RD6==0) {
                while(1);
            }
        }
    }
}
