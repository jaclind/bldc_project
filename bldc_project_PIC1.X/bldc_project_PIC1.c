// PIC16F18854 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = OFF        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = available// Scanner Enable bit (Scanner module is available for use)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
//#include <stdlib.h>
//#include <stdio.h>


#include "uart.h"
#include "bldc_queue.h"
#include "bldc_pwm.h"
#include "bldc_lib.h"


#define _XTAL_FREQ 32000000
#define STOP 0b00000000

#define nFAULT PORTCbits.RC0
#define DIR    PORTCbits.RC1
#define nBRAKE PORTCbits.RC2
#define nSLEEP PORTCbits.RC3



void interrupt ISR();

sQueue Queue;
unsigned int Array[4] = {0};


unsigned int delay_ms_count;
unsigned int hall_sensor_count;
unsigned short long speed_timer_count;

unsigned long SPEED;
unsigned long TIME;
unsigned int POWER;
unsigned long RPM;
unsigned int REV;
char MODE;

char instrBuffer = 0;
char avgIndex = 0;
char gainP = 2;  // hyffsat resultat vid 2
char gainI = 3;  //                      3


int main()
{
    

    // main-loop variables  /////////////////////////
    char instruction;
    char data;
    ///////////////////////////////////////////////
    
    iniQueue(&Queue,64);
    UART_Init(9600);
    PWMInit();
    delay_ms_init();
    speed_timer_init();
    ///////////////////////////////////////////////
   

    
    /// Hall-sensor interrupt on change
    IOCBN = 0x01;
    IOCBP = 0x01;
    IOCBF = 0;
    PIR0bits.IOCIF = 0;
    PIE0bits.IOCIE = 1;
    

    
    // PIN SETUP
  
    
    ANSELC = 0;
    ANSELB = 0;
    TRISCbits.TRISC0 = 1; // nFault input from pre driver
    TRISCbits.TRISC1 = 0; // DIR    output to pre driver
    TRISCbits.TRISC2 = 0; // nBRAKE output to pre driver
    TRISCbits.TRISC3 = 0; // nSLEEP output to pre driver
    TRISCbits.TRISC5 = 0; // PWM    output to pre driver
    TRISCbits.TRISC7 = 1; // RX
    TRISCbits.TRISC6 = 0; // TX
    RC6PPS = 0x10;        // TX on RC6
    RC5PPS = 0x0E;        // PWM6OUT on RC5
    TRISBbits.TRISB0 = 1; // Hall-sensor input from motor
    TRISBbits.TRISB1 = 1; // Hall-sensor input from motor
    TRISBbits.TRISB2 = 1; // Hall-sensor input from motor
    
    
    DIR = 1;             
    nBRAKE = 1;           
    nSLEEP = 1;           
    
    T6CONbits.T6ON = 1;
    INTCONbits.GIE = 1;
    PIE3bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    
    

    while(1)
    {        
        if(getQueueHead(&Queue,&instruction,&data))
        {           
            if(deQueue(&Queue))
            {
                switch((instruction >> 4) & 0x07)
                {
                    case 0:
                            nBRAKE = 0;
                            break;
                    case 1: 
                            MODE = 0;// Set mode to 0
                            RPM = scaler(instruction,data);                            
                            break;
                    case 2: 
                            MODE = 1;// Set mode to 1
                            POWER = ((data << 2) + (instruction & 0x03));                   
                            break;
                    case 3: 
                            TIME = scaler(instruction,data);
                            nBRAKE = 1;
                            if(instruction >= 128)
                            {
                                DIR = 1;
                            }
                            else
                            {
                                DIR = 0;
                            }
                            drive_time(TIME,RPM,POWER,MODE);                               
                            break;                           
                    case 4: 
                            REV = scaler(instruction,data);
                            nBRAKE = 1;
                            if(instruction >= 128)
                            {
                                DIR = 1;
                            }
                            else
                            {
                                DIR = 0;
                            }
                            drive_rev(REV,RPM,POWER,MODE);
                            break;
                    default:;
                }
            }         
        }        
    }
} 

void interrupt ISR()
{
    //--------------------------- TMR6 ISR ------------------------------------\\
    
    if(PIR4bits.TMR6IF)
    {
        speed_timer_count++;
        PIR4bits.TMR6IF = 0;
        T6CONbits.T6ON = 1;
    }
    
    
    //--------------------------- TMR4 ISR ------------------------------------\\
    
    if(PIR4bits.TMR4IF)
    {
        delay_ms_count++;
        PIR4bits.TMR4IF = 0;
        T4CONbits.T4ON = 1;
    } 
    
    
    //---------------------- serial reciever ISR ------------------------------\\
    
    if(PIR3bits.RCIF)
    {        
        if(instrBuffer == 0)
        {
            if(RCREG == STOP)
            {
                clrQueue(&Queue);
                TIME = 0;
                REV = 0;
                nBRAKE = 1;
                PIR3bits.RCIF = 0;
                return;
            }else
            {
                instrBuffer = RCREG;
            }
        }else
        {
            if(enQueue(&Queue,instrBuffer,RCREG))
            {
                instrBuffer = 0;
                    
            }//else
                // QUEUE FULL - SEND ERROR MESSAGE BACK VIA SERIAL
        }
        PIR3bits.RCIF = 0;
    }
    
    
    //-------------- Interrupt for hall-sensor interrupt-on-change-------------\\ 
    
    if(IOCBFbits.IOCBF0 /*|| IOCBFbits.IOCBF1 || IOCBFbits.IOCBF2*/) 
    {      
        avgPush(Array,speed_timer_count);
        speed_timer_count = 0;
               
        hall_sensor_count++;
        IOCBFbits.IOCBF0 = 0;
        IOCBFbits.IOCBF1 = 0;
        IOCBFbits.IOCBF2 = 0;
    }
}





