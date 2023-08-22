#include "header.h"
#include <stdbool.h>

int main(void)
{
    InitMCU();

    int period = 5;
    float dutyPercent = 0;
    bool changeDirection = 1;//1 for up, 0 for down
    int pressed = 0;
    
    SetPWM1R(0);
    SetPWM1RS(0);
    EnablePWM1();
    

    while(1){
        if (Button == 0 && pressed == 0) {
            pressed = 99;
            if (dutyPercent > 0.999) {
                changeDirection = 0;
            }else if (dutyPercent < 0.001) {
                changeDirection = 1;
            }
            
            if (changeDirection == 1) {
                dutyPercent += 0.2;
            }else{
                dutyPercent -= 0.2;
            }
        }else if(Button ==1){

            pressed  = 0;
        }
        SetPWM1RS((uchar)(dutyPercent * 255));

        if (changeDirection == 0) {
            LED2 = 1;
            LED3 = 0;
        }else{
            LED2 = 0;
            LED3 = 1;
        }

    }
    return 0;
}
