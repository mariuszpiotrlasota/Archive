#include "delay.h"

/**************************************************************************************************************************
procedura:
WAIT     	: zwraca adres docelowy ramki
**************************************************************************************************************************/

#define fosc_Hz  69000
#define fosc_kHz  fosc_Hz/1000

void wait_long(void)
{
    volatile unsigned int i;
    i = 50000;                              // Delay
    do (i--);
    while (i != 0);
}//wait_long



void wait(void){
  asm ("nop\n");
}//wait


void wait_10us(void)
{
    volatile unsigned int i;
    i = 1;                              // Delay
    do (i--);
    while (i != 0);
}//wait_20us


void wait_40us(void)
{
    volatile unsigned int i;
    i = 10;                              // Delay
    do (i--);
    while (i != 0);
}//wait_20us


void wait_100us(void)
{
    volatile unsigned int i;
    i = 30;                              // Delay
    do (i--);
    while (i != 0);
}//wait_20us


void wait_160us(void)
{
    volatile unsigned int i;
    i = 50;                              // Delay
    do (i--);
    while (i != 0);
}//wait_180us


void wait_500us(void)
{
    volatile unsigned short i;   
  
     i=150;
    do (i--);
    while (i != 0);
}//wait_500us



void DelayNus(unsigned int n)
{
    CCR0 = n;
    TACTL |= MC_1;          
    while(!(TACTL & BIT0));  
    TACTL &= ~MC_1;          
    TACTL &= ~BIT0;         
}



void procesorDelay(unsigned long p_ulDelay)
{
unsigned long	m_ulIndex;

	for(m_ulIndex=0; m_ulIndex < p_ulDelay; m_ulIndex++)
	{
		__no_operation();
	}
}