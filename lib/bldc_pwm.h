/*__________________________________________________________________________________
  |                                                                                |
  |Header:       bldc_pwm.h                                                        |
  |Version:      0.1                                                               |
  |Description   Functions for setup and running pwm on PIC16f18854.               |                                                                               |
  |By:           Grupp 5                                                           |
  |                                                                                |
  |Last edit:    27/4 -17, 09:40                                                   |
  |________________________________________________________________________________|
                                                                                    */
void PWMInit();                   // Setup for PWM
void PWMDutyCycle(unsigned int DutyCycle); // Sets value between 0-1023 to pwm duty cycle                                                                                    
