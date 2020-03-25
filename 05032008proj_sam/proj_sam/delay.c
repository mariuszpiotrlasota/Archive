#include "delay.h"
//clk 15MHz

//*****************************************************
void delay (unsigned int delay) 
{ 
unsigned int i;
  for(i = 0; i < delay; i++);	   
}								    							    

//****************************************************wait_ms
 void waitms (unsigned int wait_ms)  {                        /* wait function */
  int  d;

  for (d = 0; d < wait_ms*6000; d++);           /* only to delay for LED flashes */
}

//****************************************************wait_us
 void waitus (unsigned int wait_us)  {                        /* wait function */
  int  d;

  for (d = 0; d < wait_us*9; d++);           /* only to delay for LED flashes */
}

//****************************************************wait_s
 void wait (unsigned int wait_s)  {                        /* wait function */
  int  d;

  for (d = 0; d < wait_s*7000000;d++);           /* only to delay for LED flashes */
}