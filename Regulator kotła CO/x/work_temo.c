
#include  <msp430x12x2.h>
#include  "disp_seven.h"

volatile char select_com[2]={~(com1),~(com2)};
volatile char tablica[10]={s_0,s_1,s_2,s_3,s_4,s_5,s_6,s_7,s_8,s_9};
volatile char temp[25]={450,415,405,394,377,374,364,355,346,337,328,320,312,304,296,289,281,275,268,261,255,249,244,237,232,227};

//******************************************************************************
//             INIT
//******************************************************************************
void init(void){
  DIR_com = com1|com2;                                 //wyjscie na tranzystory 
  DIR_seg = BIT_ALL;                                 // segmenty LED wyjsciami
  DIR_led = led_pompa|led_klapa|led_alarm;           //ledy              
  OUT_seg = BIT_ALL;                                  //czysc segmenty led
  
  OUT_led = ~(led_pompa|led_klapa|led_alarm);           //czysc ledy
  
}

void clr_seg(void){
  OUT_com &= (com1|com2);
}

void wait(void){
  volatile unsigned int i;
  i = 60000;                              // Delay
    do (i--);
    while (i != 0);
}


void init_adc(void)
{

  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON ;
  TACCR0 = 30;                              // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
  ADC10AE |= 0x01;                          // P2.0 ADC option select
  P1DIR |= BIT2;                            // Set P1.0 to output direction
}


//******************************************************************************
//              wy�wietlacz
//******************************************************************************
void wait_seg(void)
{
  volatile unsigned int i;
  i = 100;                              // Delay
    do (i--);
    while (i != 0);
}


void send_disp(char znak_1, char znak_2)
{
  unsigned int licz=500;
  
  do
  {
        clr_seg();                           //czysc wyswietlacz
        //P1OUT=BIT7;
        OUT_seg = znak_1;                       //ustaw znak 
        OUT_com=select_com[0];               //wybierz com 
        wait_seg();                          //wyswietlaj przez czas wait 
               
        clr_seg();                           //czysc wyswietlacz
        //P1OUT=BIT6;
        OUT_seg = znak_2;                       //ustaw znak 
        OUT_com=select_com[1];               //wybierz com 
        wait_seg();                          //wyswietlaj przez czas wait 
        licz--;
  }  while (licz !=0);               
}


void set_minus(void)
{
  OUT_com=~(com1+com2);       //w�acz oba wyswietlacze
  OUT_seg = s_none;
}


//******************************************************************************
//              MAIN
//******************************************************************************
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  init();                                   // inicjalizacja portow
  init_adc();  
  
  //zmienne w programie
  volatile unsigned char seg=0;
  volatile unsigned char flaga_menu=0;
  volatile unsigned char licz=0; 
  volatile unsigned char liczba1=0;   //semnent 1 -LSB
  volatile unsigned char liczba2=0;   //segment 2 -MSB (lewy)
  
  /* FLAGA MENU:
    0 - WYSWIETL TEMPERATURE
    1 - USTAWIANIE REGULACJI
    5 - WEJSCIE DO MENU USTAW TEMP.

  */

  
 // clr_seg();    //czysc wyswietlacz
 // OUT_led ^= led_pompa; 
  
  //****************************************************
  //        p�tla While
  //****************************************************
  while(1)
  {
    
 
  switch(flaga_menu){
    //wyswietl temp.
    case 0:    
      
   

    
    //regulacja
   /*
     if (ADC10MEM > 298)     send_disp(s_none,s_1);// T<85
     if (ADC10MEM < 275)     send_disp(s_none,s_2);// T<80 
    */

    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
  
    //#############POROWNANIE , 
      if (ADC10MEM == 0)                       send_disp(s_none,s_none); // Tmax  ZAKRES :120
      if (ADC10MEM >= 1   && ADC10MEM < 200)     send_disp(s_1,s_none); // Tmax  ZAKRES :120
      if (ADC10MEM >= 201 && ADC10MEM < 231)     send_disp(s_1,s_none);// T<5 
      if (ADC10MEM >= 232 && ADC10MEM < 236)     send_disp(s_1,s_none); // T<115  
      if (ADC10MEM >= 237 && ADC10MEM < 243)     send_disp(s_1,s_none);// T<110 
      if (ADC10MEM >= 244 && ADC10MEM < 248)     send_disp(s_1,s_none);// T<105  
      if (ADC10MEM >= 249 && ADC10MEM < 254)     send_disp(s_1,s_none);// T<100 
      if (ADC10MEM >= 255 && ADC10MEM < 260)     send_disp(s_9,s_5);// T<95  
      if (ADC10MEM >= 261 && ADC10MEM < 267)     send_disp(s_9,s_0);// T<90  
      if (ADC10MEM >= 268 && ADC10MEM < 274)     send_disp(s_8,s_5);// T<85
      if (ADC10MEM >= 275 && ADC10MEM < 280)     send_disp(s_8,s_0);// T<80 
      if (ADC10MEM >= 281 && ADC10MEM < 288)     send_disp(s_7,s_5);// T<75 
      if (ADC10MEM >= 289 && ADC10MEM < 295)     send_disp(s_7,s_0);// T<70 
      if (ADC10MEM >= 296 && ADC10MEM < 303)     send_disp(s_6,s_5);// T<65 
      if (ADC10MEM >= 304 && ADC10MEM < 311)     send_disp(s_6,s_0);// T<60  
      if (ADC10MEM >= 312 && ADC10MEM < 319)     send_disp(s_5,s_5);// T<55  
      if (ADC10MEM >= 320 && ADC10MEM < 327)     send_disp(s_5,s_0);// T<50 
      if (ADC10MEM >= 328 && ADC10MEM < 336)     send_disp(s_4,s_5);// T<45 
      if (ADC10MEM >= 337 && ADC10MEM < 345)     send_disp(s_4,s_0);// T<40 
      if (ADC10MEM >= 346 && ADC10MEM < 354)     send_disp(s_3,s_5);// T<35 
      if (ADC10MEM >= 355 && ADC10MEM < 363)     send_disp(s_3,s_0);// T<30  
      if (ADC10MEM >= 364 && ADC10MEM < 373)     send_disp(s_2,s_5);// T<25 
      if (ADC10MEM >= 374 && ADC10MEM < 376)     send_disp(s_2,s_0);// T<20  
      if (ADC10MEM >= 377 && ADC10MEM < 459)     send_disp(s_1,s_5);// T<15  
      if (ADC10MEM >= temp[0] )                      send_disp(s_1,s_0);// T<10   
        
       
    
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
    TACTL = 0;      
        // licz=0;   //ustawienie menu na 1-sza pozycje
      
     break;
      
      
   
     
     
     
     
     
     
    //wejscie do menu
    case 5:
     set_minus();             //ustaw minusy
     wait();
     //*****************************wprowadznie wyboru
     //wprowadz pierwsz� liczn�
      if(licz==0){
          wait_seg();
          licz=1;                                   //nast�pna funkcja
          for(;;){                                  //petla wprowadzania wartosci wyswietlenia
             send_disp(s_none,tablica[liczba1]);              //wwyswietl            
             if(((PIN_sw & sw_up)==0)){liczba1++; if(liczba1>9)liczba1=0;};            //zwi�ksz liczbe
             if((PIN_sw & sw_down)==0){break;} 
             if(((PIN_sw & sw_up)==0) && ((PIN_sw & sw_down)==0))break;    //wyjscie z petli
             OUT_led ^=led_klapa+led_pompa;}    //toogle led
                }
     //wprowadz drug� liczn�      
      if(licz==1){
          wait_seg();
          licz=2;                                   //nast�pna funkcja
          for(;;){                                  //petla wprowadzania wartosci wyswietlenia
             send_disp(tablica[liczba2],s_none);              //wwyswietl            
             if(((PIN_sw & sw_up)==0)){liczba2++; if(liczba2>9)liczba2=0;};            //zwi�ksz liczbe
             if((PIN_sw & sw_down)==0){break;} 
             if(((PIN_sw & sw_up)==0) && ((PIN_sw & sw_down)==0))break;    //wyjscie z petli
             OUT_led ^=led_klapa+led_pompa;}   //toogle led
                }
      
     //exit
     else if(licz==2){ //wyjscie z case
          flaga_menu=0;   //w�acz wyswietlanie temp.
          OUT_led &=~(led_klapa+led_pompa);
     }
       break;

   } //THE END
       
   
   //test klawiszy
     if(((PIN_sw & sw_up)==0) && ((PIN_sw & sw_down)==0))  flaga_menu=5;     
  }
}  
