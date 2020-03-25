#include "delay.h"

/**************************************************************************************************************************
procedura:
WAIT     	: zwraca adres docelowy ramki
**************************************************************************************************************************/

#define fosc_Hz  69000
#define fosc_kHz  fosc_Hz/1000

/*
void wait_long(void)
{
    volatile unsigned int i;
    i = 50000;                              // Delay
    do (i--);
    while (i != 0);
}//wait_long


void wait_short(void)
{
    volatile unsigned int i;
    i = 5000;                              // Delay
    do (i--);
    while (i != 0);
}//wait_long


void wait(void){
  _NOP();
}//wait


void wait_10us(void)
{
  _NOP();
}//wait_20us


void wait_40us(void)
{
    volatile unsigned int i;
    i = 2;                              // Delay
    do (i--);
    while (i != 0);
   
}//wait_20us


void wait_100us(void)
{
    volatile unsigned int i;
    i = 6;                              // Delay
    do (i--);
    while (i != 0);
}//wait_20us


void wait_160us(void)
{
    volatile unsigned int i;
    i = 10;                              // Delay
    do (i--);
    while (i != 0);
}//wait_180us


void wait_500us(void)
{
    volatile unsigned short i;   
  
     i=40;
    do (i--);
    while (i != 0);
}//wait_500us


*/

void wait_long_20sek(void){
  volatile unsigned int wait20s;
  for(wait20s=0;wait20s<20;wait20s++)
  {wait_long();}
}


void wait_short(void)
{
    volatile unsigned int i;
    i = 5000;                              // Delay
    do (i--);
    while (i != 0);
}//wait_long

void wait_long2(void){
wait_long(); wait_long(); wait_long();
}


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

