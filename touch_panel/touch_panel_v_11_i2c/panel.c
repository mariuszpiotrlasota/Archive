#include  <msp430x12x.h>

#define C1 BIT3 //P1
#define B1 BIT2 //P1
#define A1 BIT1 //P1
#define C2 BIT0 //P1
#define B2 BIT4 //P2
#define A2 BIT3 //P2



//------------------------------------------------------------------------------
//                               init
//------------------------------------------------------------------------------
void init(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P3DIR |= BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1;                            // Set P1.0 to output direction
  P2DIR &= ~(BIT0+BIT1);        // linie P2.0 i P2.1 jako wejùcie
  P2DIR |= BIT4+BIT3;   //STEROWNAIE PANELEM 
  P1DIR |= 0XFF;    //STEROWANIE PANELEM + STERPWAMOE OUT 
}



//------------------------------------------------------------------------------
//                               set's 1
//------------------------------------------------------------------------------
void set1_E1(void){
  P1OUT &= (~(B1+C1+C2));
  P1OUT |= A1;
  P2OUT &= ~(B2+A2);   
}

void set1_E2(void){
  P1OUT &= (~(A1+C1+C2));
  P1OUT |= B1;
  P2OUT &= ~(B2+A2);   
}

void set1_E3(void){
  P1OUT &= (~(C1+C2));
  P1OUT |= A1+B1;
  P2OUT &= ~(B2+A2);   
}

void set1_E4(void){
  P1OUT &= (~(A1+B1+C2));
  P1OUT |= C1;
  P2OUT &= ~(B2+A2);   
}

void set1_E5(void){
  P1OUT &= (~(B1+C2));
  P1OUT |= A1+C1;
  P2OUT &= ~(B2+A2);   
}

void set1_E6(void){
  P1OUT &= (~(A1+C2));
  P1OUT |= C1+B1;
  P2OUT &= ~(B2+A2);   
}

void set1_E7(void){
  P1OUT &= (~(C2));
  P1OUT |= A1+B1+C1;
  P2OUT &= ~(B2+A2);   
}



//------------------------------------------------------------------------------
//                               set's 2
//------------------------------------------------------------------------------
void set2_E1(void){
  P1OUT &= (~(A1+B1+C1+C2));
  P2OUT &= ~(B2);  
  P2OUT |= (A2); 
}

void set2_E2(void){
  P1OUT &= (~(A1+B1+C1+C2));
  P2OUT &= ~(A2);
  P2OUT |= (B2);    
}

void set2_E3(void){
  P1OUT &= (~(A1+B1+C1+C2));
  P2OUT |= (B2+A2);   
}

void set2_E4(void){
  P1OUT &= (~(A1+B1+C1));
  P1OUT |= (C2);
  P2OUT &= ~(B2+A2); 
}

void set2_E5(void){
  P1OUT &= (~(A1+B1+C1));
  P1OUT |= (C2);
  P2OUT &= ~(B2); 
  P2OUT |= (A2);   
}

void set2_E6(void){
  P1OUT &= (~(A1+B1+C1+C2));
  P1OUT |= (C2);
  P2OUT &= ~(A2); 
  P2OUT |= (B2);   
}

void set2_E7(void){
  P1OUT &= (~(A1+B1+C1));
  P1OUT |= (C2);
  P2OUT |= (B2+A2);    
}