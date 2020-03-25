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
 
//zmienne globalne
unsigned char tmp,tmp2;  //wartoùci wys-ane/odczytane z pamieci




//******************************************************************************
//             FUNKCJA DLA I2C
//******************************************************************************

void init_i2c(void){
    I2CDir = BIT_SCL+BIT_SDA; //wyjscie
}

void wait_I2C(void){
    volatile unsigned int iii;
    iii = 1;                              // Delay
    do (iii--);
    while (iii != 0);
}

void waitx(void)
{ unsigned int zmienna_x;
   for (zmienna_x = 0; zmienna_x < 50000; zmienna_x++); 
   { 
     wait();
   } 
}

void delay_I2C(void){
    volatile unsigned int iii;
    iii = 1;                              // Delay
    do (iii--);
    while (iii != 0);
}



// funkcja generujaca sygna- start
void i2cstart(void){
  SDA_OUT; //
  SCL_OUT; // ustawienie linii SDA i SCL w tryb wyjùciowy
  SET_SDA; //
  SET_SCL; // ustawienie na liniach SDA i SCL stanu wysokiego
  delay_I2C(); // op°Ënienie
  CLR_SDA;
  delay_I2C();
  CLR_SCL;
}

// funkcja generujaca sygna- stop
void i2cstop(void){
  CLR_SDA;
  delay_I2C();
  SET_SCL;
  delay_I2C();
  SET_SDA;
  delay_I2C();
}


// funkcja wysy-aj¶ca bajt na szyn‡ I2C
void i2cwrite(unsigned char x){
  unsigned char count = 9;
  do
  {
  CLR_SCL;
  if(x & 0x80) SET_SDA; else CLR_SDA;
  x <<= 1;
  delay_I2C();
  SET_SCL;
  delay_I2C();
  }while(--count);
  CLR_SCL;
}




// funkcja odczytujaca bajt z szyny I2C
unsigned char i2cread(void){
  unsigned char count = 8;
  unsigned char temp_i2c = 0;
  
  SET_SDA; 
  SDA_IN;
  do{
    delay_I2C();
    SET_SCL;
    delay_I2C();
    temp_i2c <<= 1;
    if (I2CIn & BIT_SDA) temp_i2c++;
    CLR_SCL;
  } while (--count);
  
  SDA_OUT;
  CLR_SDA;     //potwierdzenie odebra nia danych ACLK
  delay_I2C();
  SET_SCL;
  delay_I2C();
  CLR_SCL;
  SET_SDA;
  return (temp_i2c);
}

void wpisz_data_i2c(char dana1, char dana2){
      //wpisywanie wartoùci 
        i2cstart();
        i2cwrite(0x90);     //wpisany adres pamieci na magistrali
        i2cwrite(dana1); 
        i2cwrite(dana2);        //0X00
        i2cstop();
}


void czytaj_data_i2c(char adres1){
        //wpisywanie wartoùci 

        i2cstart();
        i2cwrite(0x91);     //wpisany adres pamieci na magistrali 
        tmp = i2cread();
        tmp2 = i2cread();
       // wait_I2C();
    
        i2cstop();    
}

        


//------------------------------------------------------------------------------
                                //funkcje wysy≥ania/odbierania
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
void uart_init3(void){          //najlpesza dzia≥ajπca
  unsigned int delay_FLL;
  FLL_CTL0 |= XCAP18PF;                     // Configure load caps
  for (delay_FLL = 0xFFFF; delay_FLL > 0; delay_FLL--);  // Delay for FLL to set     -tego brakowa≥o
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
  P2DIR &= ~(sw0+sw1+sw2+sw3);        // linie P2.0 i P2.1 jako wejùcie PRZYCISK”W
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
 
  //roznica=roznica/10; //skruc liczbe      -przedzia≥ 0...100
 /*
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
  puthex(roznica);*/
}


void get_temp(void){
  //write_sent(1,"TEMP 21C",8);
  
wpisz_data_i2c(0x00,0x05);
  wait();wait();wait();wait();
 
  /*
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
   if(ADC_temp >=3429 && ADC_temp<3461) {write_sent(1,"TEM 12",2);ulamek((ADC_temp-3461)*2);}   //120
     if(ADC_temp >=3461 && ADC_temp<3490) {write_sent(1,"TEM 12",2);ulamek((ADC_temp-3490)*2);}   //125
    if(ADC_temp >=3490 && ADC_temp<3538) {write_sent(1,"TEM 13",2);ulamek((ADC_temp-3538)*2);}   //130
    if(ADC_temp >=3538 && ADC_temp<3571) {write_sent(1,"TEM 14",2);ulamek((ADC_temp-3571)*2);}   //140
    if(ADC_temp >=3571 && ADC_temp<4095) {write_sent(1,"TEM 15",2);ulamek((ADC_temp-4095)*2);}   //140   
    
      */
}


void get_temp2(void){
     czytaj_data_i2c(0x00);

     if(tmp==0x00){ write_sent(1,"TEMP   0",8);}
     if(tmp==0x01){ write_sent(1,"TEMP   1",8);}
     if(tmp==0x02){ write_sent(1,"TEMP   2",8);} 
     if(tmp==0x03){ write_sent(1,"TEMP   3",8);}
     if(tmp==0x04){ write_sent(1,"TEMP   4",8);}
     if(tmp==0x05){ write_sent(1,"TEMP   5",8);}
     if(tmp==0x06){ write_sent(1,"TEMP   6",8);}
     if(tmp==0x07){ write_sent(1,"TEMP   7",8);}
     if(tmp==0x08){ write_sent(1,"TEMP   8",8);}
     if(tmp==0x09){ write_sent(1,"TEMP   9",8);}
     if(tmp==0x0A){ write_sent(1,"TEMP  10",8);}
     if(tmp==0x0B){ write_sent(1,"TEMP  11",8);}
     if(tmp==0x0C){ write_sent(1,"TEMP  12",8);} 
     if(tmp==0x0D){ write_sent(1,"TEMP  13",8);}
     if(tmp==0x0E){ write_sent(1,"TEMP  14",8);}
     if(tmp==0x0F){ write_sent(1,"TEMP  15",8);}
     if(tmp==0x10){ write_sent(1,"TEMP  16",8);}
     if(tmp==0x11){ write_sent(1,"TEMP  17",8);}
     if(tmp==0x12){ write_sent(1,"TEMP  18",8);}
     if(tmp==0x13){ write_sent(1,"TEMP  19",8);}
     if(tmp==0x14){ write_sent(1,"TEMP  20",8);}
     if(tmp==0x15){ write_sent(1,"TEMP  21",8);}
     if(tmp==0x16){ write_sent(1,"TEMP  22",8);}
     if(tmp==0x17){ write_sent(1,"TEMP  23",8);}
     if(tmp==0x18){ write_sent(1,"TEMP  24",8);}
     if(tmp==0x19){ write_sent(1,"TEMP  25",8);}
     if(tmp==0x1A){ write_sent(1,"TEMP  26",8);}
     if(tmp==0x1B){ write_sent(1,"TEMP  27",8);}
     if(tmp==0x1C){ write_sent(1,"TEMP  28",8);}
     if(tmp==0x1D){ write_sent(1,"TEMP  29",8);}
     if(tmp==0x1E){ write_sent(1,"TEMP  30",8);}  
     if(tmp==0x1F){ write_sent(1,"TEMP  31",8);}
     if(tmp==0x20){ write_sent(1,"TEMP  32",8);}
     if(tmp==0x21){ write_sent(1,"TEMP  33",8);}
     if(tmp==0x22){ write_sent(1,"TEMP  34",8);}
     if(tmp==0x23){ write_sent(1,"TEMP  35",8);}
     if(tmp==0x24){ write_sent(1,"TEMP  36",8);}
     if(tmp==0x25){ write_sent(1,"TEMP  37",8);}
     if(tmp==0x26){ write_sent(1,"TEMP  38",8);}
     if(tmp==0x27){ write_sent(1,"TEMP  39",8);}
     if(tmp==0x28){ write_sent(1,"TEMP  40",8);}
     if(tmp==0x29){ write_sent(1,"TEMP  41",8);}
     if(tmp==0x2A){ write_sent(1,"TEMP  42",8);}
     if(tmp==0x2B){ write_sent(1,"TEMP  43",8);}
     if(tmp==0x2C){ write_sent(1,"TEMP  44",8);}
     if(tmp==0x2D){ write_sent(1,"TEMP  45",8);}
     if(tmp==0x2E){ write_sent(1,"TEMP  46",8);}
     if(tmp==0x2F){ write_sent(1,"TEMP  47",8);}
     if(tmp==0x30){ write_sent(1,"TEMP  48",8);}
     if(tmp==0x31){ write_sent(1,"TEMP  49",8);}
     if(tmp==0x32){ write_sent(1,"TEMP  50",8);}
     if(tmp==0x33){ write_sent(1,"TEMP  51",8);}
     if(tmp==0x34){ write_sent(1,"TEMP  52",8);}
     if(tmp==0x35){ write_sent(1,"TEMP  53",8);}
     if(tmp==0x36){ write_sent(1,"TEMP  54",8);}
     if(tmp==0x37){ write_sent(1,"TEMP  55",8);}
     if(tmp==0x38){ write_sent(1,"TEMP  56",8);}
     if(tmp==0x39){ write_sent(1,"TEMP  57",8);}
     if(tmp==0x3A){ write_sent(1,"TEMP  58",8);}
     if(tmp==0x3B){ write_sent(1,"TEMP  59",8);}
     if(tmp==0x3C){ write_sent(1,"TEMP  60",8);}
     if(tmp==0x3D){ write_sent(1,"TEMP  61",8);}
     if(tmp==0x3E){ write_sent(1,"TEMP  62",8);}
     if(tmp==0x3F){ write_sent(1,"TEMP  63",8);}
     if(tmp==0x40){ write_sent(1,"TEMP  64",8);}
     if(tmp==0x41){ write_sent(1,"TEMP  65",8);}
     if(tmp==0x42){ write_sent(1,"TEMP  66",8);}
     if(tmp==0x43){ write_sent(1,"TEMP  67",8);}
     if(tmp==0x44){ write_sent(1,"TEMP  68",8);}
     if(tmp==0x45){ write_sent(1,"TEMP  69",8);}
     if(tmp==0x46){ write_sent(1,"TEMP  70",8);}
     if(tmp==0x47){ write_sent(1,"TEMP  71",8);}
     if(tmp==0x48){ write_sent(1,"TEMP  72",8);}
     if(tmp==0x49){ write_sent(1,"TEMP  73",8);}
     if(tmp==0x4A){ write_sent(1,"TEMP  74",8);}
     if(tmp==0x4B){ write_sent(1,"TEMP  75",8);}
     if(tmp==0x4C){ write_sent(1,"TEMP  76",8);}
     if(tmp==0x4D){ write_sent(1,"TEMP  77",8);}
     if(tmp==0x4E){ write_sent(1,"TEMP  78",8);}
     if(tmp==0x4F){ write_sent(1,"TEMP  79",8);}
     if(tmp==0x50){ write_sent(1,"TEMP  80",8);}
     if(tmp==0x51){ write_sent(1,"TEMP  81",8);}
     if(tmp==0x52){ write_sent(1,"TEMP  82",8);}
     if(tmp==0x53){ write_sent(1,"TEMP  83",8);}
     if(tmp==0x54){ write_sent(1,"TEMP  84",8);}
     if(tmp==0x55){ write_sent(1,"TEMP  85",8);}
     if(tmp==0x56){ write_sent(1,"TEMP  86",8);}
     if(tmp==0x57){ write_sent(1,"TEMP  87",8);}
     if(tmp==0x58){ write_sent(1,"TEMP  88",8);}
     if(tmp==0x59){ write_sent(1,"TEMP  89",8);}
     if(tmp==0x5A){ write_sent(1,"TEMP  90",8);}
     if(tmp==0x5B){ write_sent(1,"TEMP  91",8);}
     if(tmp==0x5C){ write_sent(1,"TEMP  92",8);}
     if(tmp==0x5D){ write_sent(1,"TEMP  93",8);}
     if(tmp==0x5E){ write_sent(1,"TEMP  94",8);}
     if(tmp==0x5F){ write_sent(1,"TEMP  95",8);}
     if(tmp==0x60){ write_sent(1,"TEMP  96",8);}
     if(tmp==0x61){ write_sent(1,"TEMP  97",8);}
     if(tmp==0x62){ write_sent(1,"TEMP  98",8);}
     if(tmp==0x63){ write_sent(1,"TEMP  99",8);}
     if(tmp==0x64){ write_sent(1,"TEMP  100",8);}
     if(tmp==0x65){ write_sent(1,"TEMP  101",8);}
     if(tmp==0x66){ write_sent(1,"TEMP  102",8);}
     if(tmp==0x67){ write_sent(1,"TEMP  103",8);}
     if(tmp==0x68){ write_sent(1,"TEMP  104",8);}
     if(tmp==0x69){ write_sent(1,"TEMP  105",8);}
     if(tmp==0x6A){ write_sent(1,"TEMP  106",8);}
     if(tmp==0x6B){ write_sent(1,"TEMP  107",8);}
     if(tmp==0x6C){ write_sent(1,"TEMP  108",8);}
     if(tmp==0x6D){ write_sent(1,"TEMP  109",8);}
     if(tmp==0x6E){ write_sent(1,"TEMP  110",8);}

   
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
}
//------------------------------------------------------------------------------  
//      program g≥Ûwny MAIN
//------------------------------------------------------------------------------
void main(void){                                        //main

   /*inicjalizacja*/
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
 
  uart_init3();             
  LCD_init();
  switch_init();    //switch+gsm
 // ADC_init();
   init_i2c();
   wpisz_data_i2c(0x01,0x00);     //wpisz parametry konfiguracyjne 9bit
   wpisz_data_i2c(0x00,0x00);     //ustaw pomiar temp.
 // GSM_test();
  
  /*przerwania*/
  // IE1 |= URXIE0;                            // Enable USART0 RX interrupt 
  //_BIS_SR(LPM0_bits + GIE);                // Enter LPM0 w/ interrupt 
  
  /*kluczowe flagi*/
  unsigned char flaga_menu=1; 
  
  /*zmienne*/
  unsigned char licz=1;
 // char sms[]="776974616A7C7879";
  
  
  
//#################################### g≥owna pÍtla #########################

 while (1){
   if(flaga_menu==2){                   //test GSM
      
       flaga_menu=1;}
  
   if(flaga_menu==1)                   //przejdz do menu wyswietlacza
       { switch(licz)
                     {
                       case 1://MENU                                       
                         write_sent(1,LCD_MENU,sizeof(LCD_MENU));break;
                       case 2://STATUS TELEFONU
                         if((P1IN&GSM_state)==0x00){ write_sent(1,LCD_GSM_off,sizeof(LCD_GSM_off));}       
                         if(!(P1IN&GSM_state)==0x00) write_sent(1,LCD_GSM_on,sizeof(LCD_GSM_on));  
                          break;
                       case 3://TEST GSM
                         write_sent(1,LCD_TEST,sizeof(LCD_TEST));  GSM_test(); break;                
                       case 4://USTAW PARAMETRY 8-BIT PDU
                         write_sent(1,LCD_GSM,sizeof(LCD_GSM));putstr("ate0\n");
                         putstr("AT+CNMI=1,1,0,2\n");
                         break;
                       case 5://WYSLIJ INFORMACJE SMS
                         write_sent(1,LCD_SMS,sizeof(LCD_SMS));
                      //   send_SMS(tmp); 
                         break;
                       case 6:  //ON-OFF??
                        write_sent(1,LCD_GSM_on_off,sizeof(LCD_GSM_on_off));
                    //    if(GSM_now_off==1) GSM_off();
                     //   else GSM_on();
                        break;
                      }
                  flaga_menu=0;   //usun flage menu wyswietlacza
            } ;   
   if(flaga_menu==3)  
   {
    get_temp2();  
   }     

            
    if((P2IN&sw_up)==0x00){++licz; flaga_menu=2; waitx();}   //przyciski menu 
    if((P2IN&sw_down)==0x00){--licz; flaga_menu=2; waitx();} //przyciski menu 
     
    if((P2IN&sw_escape)==0x00) {
      flaga_menu=3;
      }
        
     if(licz>6) licz=1;
     if(licz<1) licz=6;
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
