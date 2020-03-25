

#include  <msp430x13x.h>


#include "delay.h"

#define PowerON   P4OUT |= (BIT0)
#define PowerOFF  P4OUT &= ~(BIT0)

#define SET_1  P4OUT |= (BIT1)
#define SET_2  P4OUT |= (BIT2)
#define CLR_1  P4OUT &= ~(BIT1)
#define CLR_2  P4OUT &= ~(BIT2)

#define S1IN  (P6IN&BIT0)


unsigned char stan;
//4.1


void test(void){
  PowerON;  wait_long();wait_long();
  PowerOFF;  wait_long();wait_long();
  SET_1;    wait_long();wait_long();
  CLR_1;    wait_long();wait_long();
  SET_2;    wait_long();wait_long();
  CLR_2;    wait_long();wait_long();
}

void test2(void){
  PowerON;  wait_short();
  
  SET_1;    wait_short();
  CLR_1;    wait_short();
  wait_long_20sek();
  
  SET_2;    wait_short();
  CLR_2;    wait_short();
  wait_long_20sek();
}

void test2debug(void){
  PowerON;  
  
  SET_1;    
   
  CLR_2; 


  CLR_1;    
 
  SET_2;   
PowerOFF;  
}

void prosedura_UP(void){
  PowerON;  wait_short();
  SET_1;    wait_short();
  CLR_2;    wait_short();
  wait_long();wait_long();
  PowerOFF;  wait_short();
}

void procedura_DOWN(void){
  PowerON;  wait_short();
  CLR_1;    wait_short();
  SET_2;    wait_short();
  wait_long();
  PowerOFF;  wait_short();
}



//*****************************************************************************
void main(void)
{
   unsigned char _flaga=0;

    WDTCTL = WDTPW + WDTHOLD;        
    P4DIR=0XFF;
    P4OUT=0X00;

    

    
   
     while(1)
     {
   

       switch(_flaga){
           case 0:
             if(S1IN!=0){_flaga=1;} //wl¹cz
             else {_flaga=2;}         //wy³¹cz
             break;
             
           case 1: prosedura_UP();  wait_long_20sek(); _flaga=0; break;
           case 2: procedura_DOWN(); wait_long_20sek();_flaga=0; break;
       }

       
       
       
       
       
       
       
       
       
       
       
     }
}




