/*__________________________________________________________________________________
  |                                                                                |
  |Header:       bldc_pwm.c                                                        |
  |Version:      0.1                                                               |
  |Description   Functions for setup and running pwm on PIC16f18854.               |                                                                               |
  |By:           Grupp 5                                                           |
  |                                                                                |
  |Last edit:    27/4 -17, 09:40                                                   |
  |________________________________________________________________________________|
                                                                                    */
#include <xc.h>
#include "bldc_pwm.h"

void PWMInit()
{
    PWM6CONbits.PWM6POL = 0;     // PWM active high
    PR2 = 0xC7;                  // Value TMR2 counts to before roll-over, 199 for 20kHz.
    T2CLKCON = 0x01;             // TMR2 uses Fosc/4
    //RC5PPS = 0x0E;               // PWM6OUT on RC5
    //TRISCbits.TRISC5 = 0;
    PWM6DCH = 0;                 // 8 MSB of Dutycycle cleared
    PWM6DCL = 0;                 // 2 LSB of DutyCycle cleared
    PIR4bits.TMR2IF = 0;         // Clear TMR2IF
    T2CON = 0b10010000;          // Prescaler 1:2, Postscaler 1:1, TMR2ON 1.
    PWM6CONbits.PWM6EN = 1;      // Enable PWM6
}

void PWMDutyCycle(unsigned int DutyCycle)
{
    PWM6DCH = DutyCycle >> 2;          // 8 MSB of DutyCycle
    PWM6DCL = (DutyCycle << 6 & 0xC0); // 2 LSB of DutyCycle

}
