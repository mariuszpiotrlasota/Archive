#include "wait.h"
#include  <msp430x16x.h>
#define t1 50000

void led_test1(void)
{
  for(volatile unsigned char a=0;a<1;a++)
  {
    P5OUT ^= BIT1;
    wait_short();
    P5OUT ^= BIT5;
    wait_short();
    P5OUT ^= BIT2;
    wait_short();
    P5OUT ^= BIT3;
    wait_short();
    P5OUT ^= BIT4;
    wait_short();
    P5OUT ^= BIT0;
    wait_short();
  }
  P5OUT = 0x00; 
  wait_short();
  for(volatile unsigned char a=0;a<1;a++)
  {
    P5OUT ^= BIT0;
    wait_short();
    P5OUT ^= BIT4;
    wait_short();
    P5OUT ^= BIT3;
    wait_short();
    P5OUT ^= BIT2;
    wait_short();
    P5OUT ^= BIT5;
    wait_short();
    P5OUT ^= BIT1;
    wait_short();
  }
  P5OUT = 0x00; 
}

void led_test2(void)
{
  for(volatile unsigned char a=0;a<4;a++)
  { 
   P5OUT ^= BIT1+BIT0;
    wait_short();
  }
 
  for(volatile unsigned char a=0;a<4;a++)
  { 
   P5OUT ^= BIT2+BIT3;
    wait_short();
  }
   
  for(volatile unsigned char a=0;a<4;a++)
  { 
   P5OUT ^= BIT5+BIT4;
    wait_short();
  }
    P5OUT = 0x00; 
}



void led_przod_on(void)
{
   P5OUT |= BIT1+BIT0; 
}

void led_tyl_on(void)
{
   P5OUT |= BIT2+BIT3; 
}

void led_bok_on(void)
{
   P5OUT |= BIT4+BIT5; 
}

void led_off(void)
{
   P5OUT = 0x00; 
}


void led_blask2(void)
{
     led_przod_on();
      wait(5000); 
      led_off();
     wait(5000); 
     led_przod_on();
      wait(5000); 
      led_off();
     wait_long();
}


void led_blask3(void)
{
     led_przod_on();
      wait(5000); 
      led_off();
     wait(5000); 
     led_przod_on();
      wait(5000); 
      led_off();
           wait(5000); 
     led_przod_on();
      wait(5000); 
      led_off();
     wait_long();
}