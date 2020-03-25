//*******************************************************************************
//  MSP-FET430P120 Demo - Software Toggle P1.0
//
//  Description: Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k
//
//                MSP430F123(2)
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include  <msp430x13x.h>
#include "display.h"

#define D8    BIT7//P1,7
#define D7    BIT6//P1,6
#define D6    BIT5//P1,5
#define D5    BIT4//P1.4
#define E     BIT2//P1,2
#define RS    BIT1//P1,1




void wait(void)
{
    volatile unsigned int i;

    i = 50000;                              // Delay
    do (i--);
    while (i != 0);
}



void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= 0XFF;                            // Set P1.0 to output direction

  for (;;)
  {
    volatile unsigned int i;

    P1OUT ^= D8;                     // Toggle P1.0 using exclusive-OR

    i = 50;                              // Delay
    do (i--);
    while (i != 0);
  }
}
