#include "ADC.h"

u_Vector2 Get_ADC_xy(){
    u_Vector2 res;
    uint timeout = 0;

    AD1CHSbits.CH0SA = 0;
    AD1CON1bits.ASAM = 1;
    while (!IFS1bits.AD1IF && timeout<1000){timeout++;}; 
    IFS1bits.AD1IF = 0;
    res.x = ADC1BUF0;

    AD1CHSbits.CH0SA = 1;
    AD1CON1bits.ASAM = 1;
    while (!IFS1bits.AD1IF && timeout<1000){timeout++;}; 
    IFS1bits.AD1IF = 0;
    res.y = ADC1BUF0;
    return res;
}

f_Vector2 Get_Stick_xy(){
    uint x_mid = 548;
    uint y_mid = 515;
    float deadzone = 0.02;
    u_Vector2 adc_res = Get_ADC_xy();
    f_Vector2 res;
    res.x = -(float)(adc_res.x -x_mid)/(float)548;
    res.y = -(float)(adc_res.y -y_mid)/(float)548;
    if (res.x<deadzone && res.x>-deadzone) {
        res.x = 0;
    }
    if (res.y<deadzone && res.y>-deadzone) {
        res.y = 0;
    }
    return res;
}
