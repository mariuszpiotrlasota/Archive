//*****************************************************************************
//
//
//
//
//*****************************************************************************
#include  <msp430x44x.h>
#include  "cyfry.h"
#include  "position.c"
//*****************************************************************************
//
//          Sterownik - made by MPL
//
//*****************************************************************************

// **********************************************  main ******************************************************** 
void main(void)
{
  volatile unsigned int zmienna_i;                  // Use volatile to prevent removal
 
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL0 |=XCAP14PF;                     // Configure load caps
 // for (zmienna_i = 0; zmienna_i < 10000; zmienna_i++);              // Delay for 32 kHz crystal to
                                            // stabilize

  LCDCTL = LCDON + LCD4MUX + LCDSG0_6;      // LCD on, 4-Mux, segments S0-S35
  BTCTL = BT_fLCD_DIV64;                   // LCD clock freq is ACLK/128
  P5SEL = 0xFC;                             // Select P5.2-7 as Com and Rxx
  clean_disp();
  unsigned char bufor_LCD[60]="MARIUSZ";
   
  for (;;)
  {   
    unsigned int count2=0; 
    //write_sent(2,&bufor_LCD[1],1);
      for(count2;count2<40;count2++)
      {
          clean_disp();
         write_sent(1,&bufor_LCD[count2],sizeof(bufor_LCD));
         //_dp3;
         wait(); wait(); wait(); wait();  wait(); wait(); wait();
      }
  }
}


