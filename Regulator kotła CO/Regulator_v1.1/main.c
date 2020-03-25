//******************************************************************************
//  MSP-FET430P120 Demo - Software Toggle P1.0
//
//******************************************************************************

#include  <msp430x12x2.h>
#include  "delay.h"
#include  "system.h"
#include  "program.h"
#include  "1wire.h"


#define Yes   1
#define No    0

//2.5 1wire


void main(void)
{
  float tem_C,tenp;
  char tem_C2,digit_1,digit_2;
  char get[10];
  procesorWTDStop();
  procesorInitIo();
 //WireInit();


  
      while(1)
      {  
          WireReset();wait_160us();
           WireWriteByte(0xcc);wait_100us();
           WireWriteByte(0x44);wait_100us(); 
           WireReset();wait_160us(); 
           WireWriteByte(0xcc);wait_100us();
           WireWriteByte(0xbe);wait_100us();
           wait_160us();wait_160us();
           get[0]=WireReadByte();
           tem_C=get[0]*0.5;
           tem_C2=(char)tem_C;
           digit_1=tem_C2/10;   //dziesiêtne
           
           digit_2=tem_C2-(digit_1*10);//jednostki
wait();

 
      }
      
      
      
      
}
