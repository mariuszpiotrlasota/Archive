#include <msp430x44x.h>
#include "link_all.h"
#define sw0 BIT0
#define sw1 BIT1
#define sw2 BIT2
#define sw3 BIT3
#define sw_up sw1 
#define sw_down sw3 
#define sw_enter sw2 
#define sw_escape sw0 
#define GSM_state BIT1 
#define GSM_on BIT5
#define CR 0x0D
#define cudz 0x22
#define CtrlZ 0x06
unsigned char read_flag=0;

 unsigned int ADCresult;
 unsigned long int DegC, Deg2;



//------------------------------------------------------------------------------
                                //funkcje wysy³ania/odbierania
//------------------------------------------------------------------------------
void puthex(char data){     
    while (!(IFG1 & UTXIFG0));    
    TXBUF0 = data;   
}

//***************** gethex 
void clr_RxBuf(void){
            UTCTL0 |= SSEL0;                        // SSEL0 = 1, no RX activity
            UTCTL0 &= ~URXSE;                       // Clear URXS signal
            UTCTL0 |= URXSE;                        // Re-enable edge detect
            UTCTL0 &= ~SSEL0;                       // SSEL0= 0, RX activity
}

void getchar(void){
          while((IFG1 & URXIFG0))                     // czekaj na znak
            if(RXBUF0=='a')   //ustawianie flag
              {
              puthex(0x05);
              }
            clr_RxBuf();
}

void message_get(char *rx_data){
  char nnn=0;
  if((IFG1 & URXIFG0)){
    clr_RxBuf();
    rx_data[nnn]=RXBUF0;
    nnn++;  }
             
  
}
//****************** putchar
int putchar (int ch){                  		
      if (ch == '\n')  {
         while (!(IFG1 & UTXIFG0));    
         TXBUF0 = CR;              	   /* output CR */
       }
      while (!(IFG1 & UTXIFG0));  
      return (TXBUF0 = ch);
}

void putstr(char *wskaz){
        while(*wskaz)
        {
         putchar(*wskaz++);
        } 
}

//------------------------------------------------------------------------------
//                              konfiguracje Uart'u
//------------------------------------------------------------------------------
void uart_init3(void){          //najlpesza dzia³aj¹ca
  unsigned int delay_FLL;
  FLL_CTL0 |= XCAP18PF;                     // Configure load caps
  for (delay_FLL = 0xFFFF; delay_FLL > 0; delay_FLL--);  // Delay for FLL to set     -tego brakowa³o
  P2SEL |= 0x30;                            // P2.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST = 1
  UTCTL0 |= SSEL1 + SSEL0 + URXSE;          // UCLK = SMCLK, start edge detect
  UBR00 = 0x36;                             // 19200 from 1Mhz
  UBR10 = 0x00;
  UMCTL0 = 0x6B;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
 }




//------------------------------------------------------------------------------    
#pragma vector=USART0RX_VECTOR                                    //przerwania
__interrupt void usart0_rx (void){
//------------------------------------------------------------------------------  
  if ((IFG1 & URXIFG0))                     // Test URXIFG0
  {
  while (!(IFG1 & UTXIFG0));              // USART0 TX buffer ready?
   TXBUF0 = RXBUF0;                        // RXBUF0 to TXBUF0
    

    
    _BIC_SR_IRQ(LPM3_bits);                 // Exit LPM3 after reti
    UTCTL0 |= SSEL0;                        // SSEL0 = 1, no RX activity
  }
  else                                      // Start edge
  {
    UTCTL0 &= ~URXSE;                       // Clear URXS signal
    UTCTL0 |= URXSE;                        // Re-enable edge detect
    _BIC_SR_IRQ(SCG1 + SCG0);               // DCO reamins on after reti
    UTCTL0 &= ~SSEL0;                       // SSEL0= 0, RX activity
  }
}


//------------------------------------------------------------------------------
    //funkcje init
//------------------------------------------------------------------------------
void LCD_init(void){
  LCDCTL = LCDON + LCD4MUX + LCDSG0_6;      // LCD on, 4-Mux, segments S0-S35
  BTCTL = BT_fLCD_DIV64;                   // LCD clock freq is ACLK/128
  P5SEL = 0xFC;                             // Select P5.2-7 as Com and Rxx
  clean_disp();
}

void switch_init(void){
  P2DIR &= ~(sw0+sw1+sw2+sw3);        // linie P2.0 i P2.1 jako wejcie PRZYCISKÓW
  P1DIR &= ~(GSM_state);
  P1DIR |=   GSM_on;

    P1OUT &= ~(GSM_on);   //gsm on
    wait();wait(); wait();wait();
    P1OUT = GSM_on;
}

void ADC_init(void){
  //adc
  ADC12CTL0 = ADC12ON+REFON+REF2_5V+SHT0_6; // Setup ADC12, ref., sampling time
  ADC12CTL1 = SHP;                          // Use sampling timer
  ADC12MCTL0 = INCH_7+SREF_1;              // Select channel A10, Vref+
  ADC12IE = 0x01;                           // Enable ADC12IFG.0
  wait2();wait2();wait2();wait2();
  ADC12CTL0 |= ENC;                         // Enable conversions
  _EINT();                                  // Enable interrupts
}

//------------------------------------------------------------------------------
//    funkcje GSM
//------------------------------------------------------------------------------
void GSM_test(void){
 //unsigned int bufor_test[8];
  putstr("ate0\n"); //echo off
  wait2();
  putstr("AT+CNMI=1,1,0,2\n");
  wait2();
  putstr("at+clip=1\n");
  wait2();
  putstr("at\n");
/*
  while((IFG1 & URXIFG0));   //sprawdzanie testu GSM
     while(RXBUF0 != 'K');clr_RxBuf();      //test OK
     write_sent(1,TEST_OK,sizeof(TEST_OK));  //informacja wstepna
     */
}

void send_SMS(char *bufor){
        putstr("at+cmgs=8\n"); 
       	  while((IFG1 & URXIFG0));                     // czekaj na znak w buforze
            while(RXBUF0 != '>');clr_RxBuf();    //krec sie puki nie natrafisz na ten znak
             putstr("0791"); putstr(CENTRALA_idea); putstr("11000B91");
             putstr(NUMER_Tato); putstr("00F68F08");
             putstr(bufor);
             putchar(0x1A);				
             putchar(0x0D); 
             wait2();wait2();wait2();
}


//------------------------------------------------------------------------------  
//      Temperatura
//------------------------------------------------------------------------------
void ulamek(unsigned char roznica){
 
  //roznica=roznica/10; //skruc liczbe      -przedzia³ 0...100
 
  if(roznica>=0  && roznica<25) write_sent_one(7,"9C",2);
  if(roznica>=25 && roznica<50) write_sent_one(7,"8C",2);
  if(roznica>=50 && roznica<75) write_sent_one(7,"7C",2);
  if(roznica>=75 && roznica<100) write_sent_one(7,"6C",2);
  if(roznica>=100 && roznica<125) write_sent_one(7,"5C",2);
  if(roznica>=125 && roznica<150) write_sent_one(7,"4C",2);
  if(roznica>=150 && roznica<175) write_sent_one(7,"3C",2);
  if(roznica>=175 && roznica<200) write_sent_one(7,"2C",2);
  if(roznica>=200 && roznica<250) write_sent_one(7,"1C",2);
  if(roznica>=250 ) write_sent_one(7,"0C",2);
  puthex(roznica);
}


void get_temp(void){
  //write_sent(1,"TEMP 21C",8);
  
  
  unsigned int ADC_temp;
  ADC12CTL0 |= ADC12SC;                   // Start conversion
  //zakresy temperatury                                               //wyliczamy roznice :
 ADC_temp=ADCresult-50;
    if(ADC_temp >=2301 && ADC_temp<2412) {write_sent(1,"TEM  0",6);ulamek((ADC_temp-2412));}   //0
    if(ADC_temp >=2412 && ADC_temp<2521) {write_sent(1,"TEM  1",6);ulamek((ADC_temp-2521)*2);}   //10
    if(ADC_temp >=2521 && ADC_temp<2627) {write_sent(1,"TEM  2",6);ulamek((ADC_temp-2574)*2);}   //20
   // if(ADC_temp >2574 && ADC_temp<2627) {write_sent(1,"TEM  2",6);ulamek((ADC_temp-2627)*2);}   //25
    if(ADC_temp >=2627 && ADC_temp<2730) {write_sent(1,"TEM  3",6);ulamek((ADC_temp-2730));}   //30
     if(ADC_temp >=2730 && ADC_temp<2830) {write_sent(1,"TEM  4",6);ulamek((ADC_temp-2830));}   //40
    if(ADC_temp >=2830 && ADC_temp<2927) {write_sent(1,"TEM  5",6);ulamek((ADC_temp-2927));}   //50
    if(ADC_temp >=2927 && ADC_temp<3020) {write_sent(1,"TEM  6",2);ulamek((ADC_temp-3020));}   //60
    if(ADC_temp >=3020 && ADC_temp<3110) {write_sent(1,"TEM  7",2);ulamek((ADC_temp-3110));}   //70
    if(ADC_temp >=3110 && ADC_temp<3196) {write_sent(1,"TEM  8",2);ulamek((ADC_temp-3196));}   //80
    if(ADC_temp >=3196 && ADC_temp<3279) {write_sent(1,"TEM  9",2);ulamek((ADC_temp-3279));}   //90
    if(ADC_temp >=3279 && ADC_temp<3358) {write_sent(1,"TEM 10",2);ulamek((ADC_temp-3358));}   //100
    if(ADC_temp >=3358 && ADC_temp<3429) {write_sent(1,"TEM 11",2);ulamek((ADC_temp-3429)*2);}   //110
  /* if(ADC_temp >=3429 && ADC_temp<3461) {write_sent(1,"TEM 12",2);ulamek((ADC_temp-3461)*2);}   //120
     if(ADC_temp >=3461 && ADC_temp<3490) {write_sent(1,"TEM 12",2);ulamek((ADC_temp-3490)*2);}   //125
    if(ADC_temp >=3490 && ADC_temp<3538) {write_sent(1,"TEM 13",2);ulamek((ADC_temp-3538)*2);}   //130
    if(ADC_temp >=3538 && ADC_temp<3571) {write_sent(1,"TEM 14",2);ulamek((ADC_temp-3571)*2);}   //140
    if(ADC_temp >=3571 && ADC_temp<4095) {write_sent(1,"TEM 15",2);ulamek((ADC_temp-4095)*2);}   //140   
*/}

//------------------------------------------------------------------------------  
//      program g³ówny MAIN
//------------------------------------------------------------------------------
void main(void){                                        //main

   /*inicjalizacja*/
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  uart_init3();             
  LCD_init();
  switch_init();    //switch+gsm
  ADC_init();
 // GSM_test();
  
  /*przerwania*/
  // IE1 |= URXIE0;                            // Enable USART0 RX interrupt 
  //_BIS_SR(LPM0_bits + GIE);                // Enter LPM0 w/ interrupt 
  
  /*kluczowe flagi*/
  unsigned char flaga_menu=1; 
  
  /*zmienne*/
  unsigned char licz=1;
  char sms[]="776974616A7C7879";
  
 // while(1)
  //{
   // get_temp();  
   // wait2();     wait2();    wait2();   
   //  wait2();    wait2();    wait2();      wait2();    wait2();    wait2();
  //}

  
  
  
//#################################### g³owna pêtla #########################

 while (1){
   if(flaga_menu==2){                   //test GSM
       GSM_test();
       flaga_menu=1;}
  
   if(flaga_menu==1)                   //przejdz do menu wyswietlacza
       { switch(licz)
                     {
                       case 1:                                        
                         write_sent(1,LCD_MENU,sizeof(LCD_MENU));break;//MENU
                       case 2:
                          if((P1IN&GSM_state)==0x00) write_sent(1,LCD_GSM_off,sizeof(LCD_GSM_off));         //STAN  GSM'U ON/OFF
                          if(!(P1IN&GSM_state)==0x00) write_sent(1,LCD_GSM_on,sizeof(LCD_GSM_on));  
                          break;
                       case 3:
                         write_sent(1,LCD_NUMER,sizeof(LCD_NUMER));break;                
                       case 4:
                         write_sent(1,LCD_GSM,sizeof(LCD_GSM));putstr("ate0\n");
                         putstr("AT+CNMI=1,1,0,2\n");
                         break;
                       case 5:
                         write_sent(1,LCD_MEM,sizeof(LCD_MEM));

                         break;
                     }
                  flaga_menu=0;   //usun flage menu wyswietlacza
            } ;   
    
            
    if((P2IN&sw_up)==0x00){++licz; flaga_menu=2; wait2();}   //przyciski menu 
    if((P2IN&sw_down)==0x00){--licz; flaga_menu=2; wait2();} //przyciski menu 
     
    if((P2IN&sw_escape)==0x00) {
         get_temp();  
         wait2();     wait2();    wait2();   wait2();  
         wait2();    wait2();    wait2();      wait2();      wait2();}
    
    
     if(licz>5) licz=1;
     if(licz<1) licz=5;
  }                                                                   //end while


}

//------------------------------------------------------------------------------  
//      przerwania
//------------------------------------------------------------------------------
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  ADCresult = ADC12MEM0;                    // Move results, IFG is cleared
 
}
