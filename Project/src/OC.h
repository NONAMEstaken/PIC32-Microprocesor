#ifndef OC
#define OC

#include "global.h"

// pwm 1 use OC3
void Enable_PWM1();
void Disable_PWM1();
void Set_PWM1R(uint val);
void Set_PWM1RS(uint val);

//pwm 2 use OC4
void Enable_PWM2();
void Disable_PWM2();
void Set_PWM2R(uint val);
void Set_PWM2RS(uint val);

#endif // !OC

