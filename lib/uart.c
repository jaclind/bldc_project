#include <xc.h>
#include "uart.h"
#define _XTAL_FREQ 32000000

char UART_Init(const long int baudrate)
{
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);
	if(x>255)
	{
		x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
		BRGH = 1;
	}
	if(x<256)
	{
	  SPBRG = x;
	  TX1STAbits.SYNC = 0;
      //TRISCbits.TRISC7 = 1; // RX
      //TRISCbits.TRISC6 = 0; // TX
      //RC6PPS = 0x10;        // TX on RC6
	  SPEN = 1;
      CREN = 1;
      TXEN = 1;
	  return 1;
	 }
	 return 0;
}

char UART_TX_Empty()
{
  return TRMT;
}

char UART_Data_Ready()
{
   return RCIF;
}
char UART_Read()
{
  while(!RCIF);
  return RCREG;
}

void UART_Read_Text(char *Output, unsigned int length)
{
	for(int i=0;i<length;i++)
	Output[i] = UART_Read();
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
	  UART_Write(text[i]);
}
