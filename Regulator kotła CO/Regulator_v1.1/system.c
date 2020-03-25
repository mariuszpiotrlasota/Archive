#include  <msp430x12x2.h>
#include "delay.h"
#include "system.h"

/**************************************************************************************************************************
procedura:
procesorWTDStop 	: zwraca adres docelowy ramki
**************************************************************************************************************************/
void procesorClkSetup(void)
{
  //SR=BIT1;
  DCOCTL &=~(0XFF);
  BCSCTL1 =BIT7;
  BCSCTL1 =BIT7|BIT3;
}//procesorClkSetup


/**************************************************************************************************************************
procedura:
procesorWTDStop 	: zwraca adres docelowy ramki
**************************************************************************************************************************/
void procesorWTDStop(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
}//procesorWTDStop


/**************************************************************************************************************************
procedura:
initIO		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void procesorInitIo(void)
{
  P1DIR =0x00;
  P2DIR =0x00;
  P3DIR =0x00;
 
  P1OUT =0x00;
  P2OUT =0x00;
  P3OUT =0x00;  
 
  P1DIR |=  BIT2+BIT3;                            // Set P1.0 to output direction


  DCOCTL=0XFF;    //OSILLATOT
  BCSCTL1=BIT1+BIT2+BIT3;
}//procesorInitIo


/**************************************************************************************************************************
procedura:
Led_test		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void led_test(void)
{
  dir_led1|=Led1;
  port_led1^=Led1;
  wait();
}


