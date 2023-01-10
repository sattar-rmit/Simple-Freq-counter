/*


ARDUINO FREQUENCY  COUNTER
WRITTEN BY:   ABDUL SATTAR NOONARI
FREQUENCY INPUT PIN = ARDUINO DIGITAL PIN 5.


REMARKS:
TIMER2 WITH NO PRESCALING MAKES TIMING CALCULATION EVEN MORE ACCURATE,
WITH PRECISION OF NANOSECONDS.

HAVE FUN.
ALL RIGHTS RESERVED TO Mr. ABDUL SATTAR 
COPYRIGHT(2010).

*/




#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13,11,10,9,8,7 );

#define T1 5

volatile unsigned int	T1_overflow=0;
volatile unsigned int	T2_overflow=0;
         unsigned int	T1_value=0;
         unsigned long   T1_count=0;

float	Freq=0;










SIGNAL(SIG_OVERFLOW1)
{
T1_overflow++;
}


SIGNAL(SIG_OVERFLOW2)
{

  T2_overflow++;

}



void setup()

{
TCCR1A=0x00;
TCCR1B=0x07;            	 //SET TO COUNT T1_PIN on falling edge0x06,  0x07 for rising edge
TCCR2A=0x00;
TCCR2B=0x07;			//TIMER2 CLOCK INPUT WITH NO PRESCALER=0x01    div by 8 presclr=0x02,   div by 64=0x04,,,,, div by 1024==0x07
TIMSK1=0x01;			//enable interrupts for T1 and T0
TIMSK2=0x01;
pinMode(T1, INPUT);
sei();

}





void loop()
{


if(T2_overflow==65)
    {
    T1_value=TCNT1;

    cli();



//   T2  with no prescaler  256*62.5nS=16000nS=16us  ----->
//255*62.5ns=15,937.5 ns  =    15.9375us ,  such 10000 increments 10000*15.9375us = 159375us  =  159.375ms  =.159375sec  --->   such 65535 ovfs   65535 *15.9375us=  seconds


//timer2 with 0x04= div by 64 presclr,,,,,,clock .25mhz=250khz,,,,,4uS time period,,,,,,,255*4=1020uS=1.02mS,,,,,,
//1020*100ovrflws=102ms


//   T2 with TCCR2B=0x02 div by 8 prescaler   , 2mhz clk inpt,   T=500nS.    255*500=127.500uS,   10,000 ovrflws    10000*127.5=1.275000seconds 

// T2   presclr div by 1024=====0x07,,,, 15.625khz,,,,,,,,,,T=64us,,,,,,255*64us=16.32ms,,,,,,, 65overflows-->65*16.32ms=1.0608sec

    T1_count=T1_value+(T1_overflow*65535);
    Freq=T1_count/1.06081;




 


    lcd.begin(16,2);
    lcd.print("Frequency:(Hz)");
    lcd.setCursor(0,1);
    lcd.print(Freq,DEC);





    T1_overflow=0;
    T2_overflow=0;


    TCCR1A=0x00;
    TCCR1B=0x07;            	 //SET TO COUNT T1_PIN on falling edge0x06,  0x07 for rising edge
    TCCR2A=0x00;
    TCCR2B=0x07;			//TIMER1 CLOCK INPUT WITH NO PRESCALER=0x01    div by 8 presclr=0x02
    sei();
    TCNT1=0x00;
    TCNT2=0x00;



	}


}
