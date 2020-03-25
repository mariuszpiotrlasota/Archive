

#include  <msp430x13x.h>
#include "lcd_4b.h"
#include "system.h"
#include "delay.h"
#include "program.h"
#include "1wire.h"
#include "PCF8593.h"



//4,0
#define S1IN  (P2IN&BIT6)
#define S2IN  (P2IN&BIT7)


#define _StanPRACA  1
#define _StanPrograTemREGULACJI  2
#define _StanPrograTemWODY  3
#define _StanPrograPRZEDMUCH 4
#define _StanPrograGODZINY  5
#define _StanPrograMINUTY 6
#define _StanPrograPCF8593 7
#define _StanPrograWYGASZANIE 8
#define _StanPrograFLASHdata 9
#define _StanROZPALANIE  11
#define _StanWYGASZANIE  12




void main(void)
{
  unsigned char _flaga=0, _menu=1, _stan=1;
 
    
    
  procesorWTDStop();
  procesorSetInternalCLK();
  
  //inicjalizacja 
  lcd_init();                         
  init_i2c();  
  initDrivePort();
  ResetPCF8593();
  

   
   
     while(1)
     {
       

       
       switch(_flaga){
         //*****
           case 0:   
             ReadFlashMSP430();
             ShowLogoOnLCD();
             set_main_text();
             _flaga=_StanPRACA;
              break;
         
         
         //*********************************_StanPRACA
         case _StanPRACA:    
              StanPRACA();
              ShowTimeOnLCD();
      
              
              
              if(S1IN==0){wait_long(); if((S1IN==0)&&(S2IN==0)){} else{_menu++;}} 
              if(S2IN==0){wait_long(); if((S1IN==0)&&(S2IN==0)){} else{_menu--;}} //ZMIEN STAN : ROZPALANIE/PRACA/WYGASZANIE   
                if(_menu>4){_menu=1;}if(_menu<1){_menu=4;}
              
              
              
              
              
    
            
              
              lcd_putstr_goto("",4,11);
              switch(_menu){
                  case 1: lcd_putstr(" :WYGASZA "); _stan=1; break;
                  case 2: lcd_putstr(" :REGULAC."); _stan=2; break;
                  case 3: lcd_putstr(" :WODA    "); _stan=3; break;
                  case 4: lcd_putstr(" :CZAS    "); _stan=4; break;
              }
              
                
              
              
              if((S1IN==0)&&(S2IN==0)){
                wait_long();
                if((S1IN==0)&&(S2IN==0)){
                  
                  
                   switch(_stan){
                    case 1: _flaga=_StanPrograWYGASZANIE; break;
                    case 2: _flaga=_StanPrograTemREGULACJI; break;
                    case 3: _flaga=_StanPrograTemWODY; break;
                    case 4: _flaga=_StanPrograGODZINY; break;
                    }             
                }
             
             
             
                
                
              }               
              break;

              
              
           
              
              
              
              
              
              
              
         //*********************************_StanPrograWYGASZANIE
         case _StanPrograWYGASZANIE:   
              setTemWygaszKotlaOnLCD();
              if((S1IN==0)&&(S2IN==0)){wait_long();if((S1IN==0)&&(S2IN==0)){_flaga=_StanPrograFLASHdata;}}
              break;
         
              
              
        //*********************************_StanPrograTemREGULACJI
        case _StanPrograTemREGULACJI:     
              setTemRegKotlaOnLCD();
              if((S1IN==0)&&(S2IN==0)){wait_long();if((S1IN==0)&&(S2IN==0)){_flaga=_StanPrograFLASHdata;}}
              break;

              
         //*********************************_StanPrograTemWODY
         case _StanPrograTemWODY:   
              setTemRegWodyOnLCD();
              if((S1IN==0)&&(S2IN==0)){wait_long();if((S1IN==0)&&(S2IN==0)){_flaga=_StanPrograFLASHdata;}}
              break;
              
              
        //*********************************_StanPrograPRZEDMUCH
         case _StanPrograPRZEDMUCH:   
              SetPrzedmuch();
              if((S1IN==0)&&(S2IN==0)){wait_long();if((S1IN==0)&&(S2IN==0)){_flaga=_StanPrograFLASHdata;}}
              break;
         
              
        //*********************************_StanPrograFLASHdata
         case _StanPrograFLASHdata:   
              WriteFlashMSP430Data();
              if((S1IN==0)&&(S2IN==0)){wait_long2();_flaga=_StanPRACA;}
              break;              
   
       //*********************************
       case _StanPrograGODZINY:    //_StanPrograGODZINY
         
              SetTimeGodzOnLCD();
              if((S1IN==0)&&(S2IN==0)){wait_long2(); _flaga=_StanPrograMINUTY;}
              break;  
              
       case _StanPrograMINUTY:    //_StanPrograMINUTY
              SetTimeMinOnLCD();
              if((S1IN==0)&&(S2IN==0)){wait_long2(); _flaga=_StanPrograPCF8593;}
              break;   
              
       case _StanPrograPCF8593:     //_StanPrograPCF8593    
              StoreDataOnPCF8593();
              _flaga=_StanPRACA;
              break; 
              
              
              
              
              
              
              
              
              
              
        //*********************************_StanROZPALANIE  -NIE AKTYWNA
        case _StanROZPALANIE:    
              StanROZPAL();    
              if(S1IN==0){wait_long2(); _flaga=6;}
              if(S2IN==0){wait_long2(); _flaga=0;}    
              wait_long2();wait_long2();wait_long2();
              _flaga=_StanPRACA;
              break;
              
              
              
              
        //*********************************_StanWYGASZANIE  -NIE AKTYWNA
        case _StanWYGASZANIE:    
              StanWYGASZ();
              if(S1IN==0){wait_long2(); _flaga=5;}
              if(S2IN==0){wait_long2(); _flaga=0;}
              wait_long2();wait_long2();wait_long2();
              _flaga=_StanPRACA;
              
                 
            
           
              
              break;              
              
              
              
              
  
    
              
              
              
       }
       
       
    
           
           
     }
}




