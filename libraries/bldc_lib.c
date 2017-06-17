#include "bldc_lib.h"
#include "bldc_pwm.h"
#include <xc.h>
#define PWM_OFF 0
#define NBR_OF_POLES 1
#define _XTAL_FREQ 32000000
#define TIME_CONST 10

unsigned int scaler(char comm, char data)
{
    return data << (comm & 0x0F);
}





void speed_timer_init()
{
    T6CON = 0b00000000;     // Prescaler 1:1
    T6PR = 0xFF;               // Value for timer roll-over
    T6CLKCON = 0x01;        // Source clock fosc/4
    PIR4bits.TMR6IF = 0;    // Clear IF
    PIE4bits.TMR6IE = 1;    // Enable interrupt
}


void delay_ms_init()        // Uses TMR4 to cause interrupt every 1 ms
{
    T4CON = 0b01100000;     // Prescaler 1:64
    T4PR = 125;             // Value for timer roll-over
    T4CLKCON = 0x01;        // Source clock fosc/4
    PIR4bits.TMR4IF = 0;    // Clear IF
    PIE4bits.TMR4IE = 1;    // Enable interrupt.  
}

void delay_ms_start()
{
    delay_ms_count = 0;     // Reset counter   
    T4CONbits.T4ON = 1;     // Turn on timer
}




void drive_time(unsigned long tTIME, unsigned long tRPM, unsigned int tPOWER, char tMODE)
{
    int integral = 0;
    
    if(tMODE == 0) // RPM
    {
        delay_ms_start();      //start timer
        while(delay_ms_count <= TIME)
        {
            PID(&integral);    
        }
        
        PWMDutyCycle(PWM_OFF);
        T4CONbits.ON = 0;//timer off
    }
    else if(tMODE == 1) // POWER-mode
    {
        PWMDutyCycle(tPOWER);
        delay_ms_start(); // start timer
       
        while(delay_ms_count <= TIME);
        PWMDutyCycle(PWM_OFF);
        T4CONbits.ON = 0;//timer off    
    }
}

void drive_rev(unsigned int rREV, unsigned long rRPM, unsigned int rPOWER, char rMODE)
{
    int integral = 0;
    
    if(rMODE == 0) // RPM
    {
        while(hall_sensor_count <= (REV*NBR_OF_POLES))
        {
            PID(&integral);
        }
        PWMDutyCycle(PWM_OFF);                       
    }
    else if(rMODE == 1) // POWER-mode
    {
        hall_sensor_count = 0; // reset sensor counter
        PWMDutyCycle(rPOWER);  // Apply PWM-duty cycle
        while(hall_sensor_count <= (REV*NBR_OF_POLES)); // Count number of revolutions
        PWMDutyCycle(PWM_OFF); // Turn motor off   
    }
}

void avgPush(unsigned int avgArray[], unsigned long int value)
{
    if(avgIndex < 4)
    {
        avgArray[avgIndex++] = value;
    }
    else
    {
        avgIndex = 0;
        avgArray[avgIndex++] = value;       
    }
}

void PID(int *PIDintegral)
{
    int output;
    int error;
    
    __delay_ms(TIME_CONST);
     error = (((Array[0] >> 1) + (Array[1] >> 1) + (Array[2] >> 1) + (Array[0] >> 1)) - RPM);
     *PIDintegral = *PIDintegral + (error >> gainI);
     if(*PIDintegral > 1023)
     {
        *PIDintegral = 1023;
     }
     if((!Array[0]))
     {
        Array[0] = 30000;
     }
            
     output = (error >> gainP) + *PIDintegral;       
     
     if(output > 1023)
     {
        output = 1023;
     }
     if(output < 0)
     {
        output = 0;
     }
     PWMDutyCycle(output);                
}