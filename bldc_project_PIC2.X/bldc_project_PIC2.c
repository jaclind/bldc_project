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
#define _XTAL_FREQ  32000000
#define DIR PORTCbits.RC3
#define nBRAKE PORTCbits.RC4



int main(void) {
    
    DIR = PORTCbits.RC3;
    nBRAKE = PORTCbits.RC4;
       
    
    TRISB = 0x00;
    TRISCbits.TRISC0 = 1;   // Hall-sensor input                        
    TRISCbits.TRISC1 = 1;   // Hall-sensor input
    TRISCbits.TRISC2 = 1;   // Hall-sensor input
    TRISCbits.TRISC3 = 1;   // DIR input from PIC1
    TRISCbits.TRISC4 = 1;   // nBRAKE input from PIC1
    
    
    ANSELB = 0;
    ANSELC = 0;
    char value = 0;
    
    while(1)
    {
        
        while(nBRAKE == 0)
        {
            PORTB = 0b00111000;
        }
        
        if(DIR == 1)
        {
            value = PORTC & 0b00000111;
            switch(value)
            {
                case 1: PORTB = 0b00001010; break;
                case 2: PORTB = 0b00010100; break;
                case 3: PORTB = 0b00001100; break;
                case 4: PORTB = 0b00100001; break;
                case 5: PORTB = 0b00100010; break;
                case 6: PORTB = 0b00010001; break;
                default: PORTB = 0;
            }
        }
        if(DIR == 0)
        {
            value = PORTC & 0b00000111;
            switch(value)
            {
                case 6: PORTB = 0b00001010; break;
                case 5: PORTB = 0b00010100; break;
                case 4: PORTB = 0b00001100; break;
                case 3: PORTB = 0b00100001; break;
                case 2: PORTB = 0b00100010; break;
                case 1: PORTB = 0b00010001; break;
                default: PORTB = 0;
            }
        }      
    }
    return 0;
}




