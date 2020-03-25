//*********************************************
// 
//*********************************************

#include  <msp430x12x2.h>
#include  "disp_seven.h"
#include  "i2c.h"


 char select_com[2]={(com1),(com2)};
 char tablica[10]={s_0,s_1,s_2,s_3,s_4,s_5,s_6,s_7,s_8,s_9};
 
//zmienne globalne
unsigned char tmp;  //wartoœci wys³ane/odczytane z pamieci




//******************************************************************************
//             FUNKCJA DLA I2C
//******************************************************************************

void init_i2c(void){
    I2CDir = BIT_SCL+BIT_SDA;
}

void wait_I2C(void){
    volatile unsigned int i;
    i = 1;                              // Delay
    do (i--);
    while (i != 0);
}


void delay_I2C(void){
    volatile unsigned int i;
    i = 1;                              // Delay
    do (i--);
    while (i != 0);
}



// funkcja generujaca sygna³ start
void i2cstart(void){
  SDA_OUT; //
  SCL_OUT; // ustawienie linii SDA i SCL w tryb wyjœciowy
  SET_SDA; //
  SET_SCL; // ustawienie na liniach SDA i SCL stanu wysokiego
  delay_I2C(); // opóŸnienie
  CLR_SDA;
  delay_I2C();
  CLR_SCL;
}

// funkcja generujaca sygna³ stop
void i2cstop(void){
  CLR_SDA;
  delay_I2C();
  SET_SCL;
  delay_I2C();
  SET_SDA;
  delay_I2C();
}


// funkcja wysy³aj¹ca bajt na szynê I2C
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
  unsigned char count = 8, temp = 0;
  SET_SDA; 
  SDA_IN;
  do{
    delay_I2C();
    SET_SCL;
    delay_I2C();
    temp <<= 1;
    if (P1IN & BIT_SDA) temp++;
    CLR_SCL;
  } while (--count);
//  if(ack) SET_SDA;
  //delay();
  //SET_SCL;
  //delay();
  //CLR_SCL;
  return (temp);
}

void wpisz_data_i2c(char adres, char dana1){
      //wpisywanie wartoœci 
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres); 
        i2cwrite(dana1);        //0X00
        i2cstop();
}


void czytaj_data_i2c(char adres1){
        //wpisywanie wartoœci 
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres1); 
        i2cstop();   
        i2cstart();
        i2cwrite(0xA1);     //wpisany adres pamieci na magistrali 
//------wartoœæ 1
        tmp = i2cread();
        wait_I2C();
        i2cstop();    
}

        


        
        

//******************************************************************************
//             INIT
//******************************************************************************
void init(void){
  DIR_com = com1|com2;                                 //wyjscie na tranzystory 
  DIR_seg = BIT_ALL;                                 // segmenty LED wyjsciami
  DIR_led = led_pompa|led_klapa|led_alarm;           //ledy              
  OUT_seg = BIT_ALL;                                  //czysc segmenty led
  OUT_led &= ~(led_pompa|led_klapa|led_alarm);           //czysc ledy
  DIR_SW &= ~(sw_down+sw_up); 

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


void init_adc(void){
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON ;
  TACCR0 = 30;                              // Delay dla Ref 
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
  ADC10AE |= 0x01;                          // P2.0 ADC option select
  P1DIR |= BIT2;                            // Set P1.0 to output direction
}


//******************************************************************************
//              wyœwietlacz
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
  unsigned int licz=300;
  
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
  OUT_com=~(com1+com2);       //w³acz oba wyswietlacze
  OUT_seg = s_none;
}


//******************************************************************************
//              MAIN
//******************************************************************************
void main(void)
{
  OUT_led &=~(led_klapa+led_pompa+led_alarm); //gaœ);
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
                                  // inicjalizacja portow
  init_adc(); 
   OUT_led &=~(led_klapa+led_pompa+led_alarm); //gaœ);
  init_i2c();
   OUT_led &=~(led_klapa+led_pompa+led_alarm); //gaœ);
  init(); 
  
  
  //zmienne w programie
  volatile unsigned char seg=0;
  volatile unsigned char flaga_menu=0;
  volatile unsigned char licz=0; 
  volatile unsigned char liczba1=0;   //semnent 1 -LSB
  volatile unsigned char liczba2=0;   //semnent 1 -LSB
  volatile unsigned char pompa_on;
  volatile unsigned char temperatura;

  /* FLAGA MENU:
    0 - WYSWIETL TEMPERATURE
    1 - REGULACJIA
    2 - REGULACJA POMPY I KLAPY
    5 - WEJSCIE DO MENU USTAW TEMP.
  */
 

  //****************************************************
  //        pêtla While
  //****************************************************
  while(1)
  {
    
 
  switch(flaga_menu){
    
 case 0:                                  //wyswietl temp.    0 - WYSWIETL TEMPERATURE
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
     //#############POROWNANIE , 
      if (ADC10MEM == 0)                       send_disp(s_none,s_none); // Tmax  ZAKRES :120
      if (ADC10MEM >= 1  && ADC10MEM < t120)     send_disp(s_1,s_none); // Tmax  ZAKRES :120
      if (ADC10MEM >= t120 && ADC10MEM < t115){temperatura=100;    send_disp(s_1,s_none);}// T<5 
      if (ADC10MEM >= t115 && ADC10MEM < t110){temperatura=100;    send_disp(s_1,s_none);} // T<115  
      if (ADC10MEM >= t110 && ADC10MEM < t105){temperatura=95;    send_disp(s_9,s_5);}// T<110 
      if (ADC10MEM >= t105 && ADC10MEM < t100){temperatura=90;    send_disp(s_9,s_0);}// T<105  
      if (ADC10MEM >= t100 && ADC10MEM < t95) {temperatura=85;    send_disp(s_8,s_5);}// T<100 
      if (ADC10MEM >= t95 && ADC10MEM < t90)  {temperatura=80;    send_disp(s_8,s_0);}// T<95  
      if (ADC10MEM >= t90 && ADC10MEM < t85)  {temperatura=75;    send_disp(s_7,s_5);}// T<90  
      if (ADC10MEM >= t85 && ADC10MEM < t80)  {temperatura=70;   send_disp(s_7,s_0);}// T<85
      if (ADC10MEM >= t80 && ADC10MEM < t75)  {temperatura=65;     send_disp(s_6,s_5);}// T<80 
      if (ADC10MEM >= t75 && ADC10MEM < t70)  {temperatura=60;     send_disp(s_6,s_0);}// T<75 
      if (ADC10MEM >= t70 && ADC10MEM < t65)  {temperatura=55;     send_disp(s_5,s_5);}// T<70 
      if (ADC10MEM >= t65 && ADC10MEM < t60)  {temperatura=50;     send_disp(s_5,s_0);}// T<65 
      if (ADC10MEM >= t60 && ADC10MEM < t55)  {temperatura=45;     send_disp(s_4,s_5);}// T<60  
      if (ADC10MEM >= t55 && ADC10MEM < t50)  {temperatura=40;     send_disp(s_4,s_0);}// T<55  
      if (ADC10MEM >= t50 && ADC10MEM < t45)  {temperatura=35;     send_disp(s_3,s_5);}// T<50 
      if (ADC10MEM >= t45 && ADC10MEM < t40)  {temperatura=30;     send_disp(s_3,s_0);}// T<45 
      if (ADC10MEM >= t40 && ADC10MEM < t35)  {temperatura=25;     send_disp(s_2,s_5);}// T<40 
      if (ADC10MEM >= t35 && ADC10MEM < t30)  {temperatura=20;     send_disp(s_2,s_0);}// T<35 
      if (ADC10MEM >= t30 && ADC10MEM < t25)  {temperatura=15;     send_disp(s_1,s_5);}// T<30  
      if (ADC10MEM >= t25 && ADC10MEM < t20)  {temperatura=10;     send_disp(s_1,s_0);}// T<25 
      if (ADC10MEM >= t20 && ADC10MEM < t15)  {temperatura=5;     send_disp(s_0,s_5);}// T<20  
      if (ADC10MEM >= t15 && ADC10MEM < t10)  {temperatura=0;     send_disp(s_0,s_0);}// T<15  
      if (ADC10MEM >= t10 )                   {temperatura=0;     send_disp(s_0,s_0);}// T<10   
          licz=0;   //ustawienie menu na 1-sza pozycje
          flaga_menu=1;

      break;
     
  case 1:
     czytaj_data_i2c(0x02);    //ADRES 1, ADRES2
     // czytaj_data_i2c2(0x04);    //ADRES 1, ADRES2
      //w³¹cz pompe wody po przekroczeniu temp. > 40*C
   
     if (temperatura>=40) pompa_on=1;  //zapal gdy powy¿ej 
        // if(temperatura<=25) OUT_led &= ~led_klapa; //zamknij klapê
         else
          pompa_on=0;
         
 
        
         
         //tmp=4;
       //w³¹cz klapy
        if((temperatura/10)>=tmp) 
        {
          OUT_led &= ~led_klapa; //zamknij klapê
          OUT_led |=(led_pompa);  //ZGAS POMPE
        }
        else
        { 

   if(temperatura<=25) OUT_led &= ~led_klapa; //zamknij klapê  
   else
          OUT_led |= (led_klapa);  //otwórz klapê       
          if(pompa_on==1)
              OUT_led |= led_pompa;
          else 
              OUT_led &=~(led_pompa);  
        }
    
    //czyœæ wartoœæ w przetworniku ADC0
       TACTL = 0;  
       flaga_menu=0;
    break;

     
    //wejscie do menu
  case 5:
         set_minus();             //ustaw minusy
         wait();
         //wprowadznie wyboru
         
         //wprowadz drug¹ licznê      
          if(licz==0){
              //wait_seg();
              licz=1;                                   //nastêpna funkcja
               czytaj_data_i2c(0x02);    //ADRES 1, ADRES2
               liczba1=tmp;
              for(;;){                                  //petla wprowadzania wartosci wyswietlenia
                 send_disp(tablica[liczba1],s_none);              //wwyswietl            
                 if(((PIN_sw & sw_up)==0)){liczba1++; if(liczba1>9)liczba1=0;};            //zwiêksz liczbe
                 if((PIN_sw & sw_down)==0){
                   wpisz_data_i2c(0x02,liczba1);  //ADRES,DARA1,DATA2
                   break;} 
                 if(((PIN_sw & sw_up)==0) && ((PIN_sw & sw_down)==0))break;    //wyjscie z petli
                 OUT_led ^=led_alarm;}   //toogle led
                    }
         //wprowadz pierwsz¹ licznê
      /* if(licz==1){
              //wait_seg();
              licz=2;                                   //nastêpna funkcja
                                 //nastêpna funkcja
               czytaj_data_i2c2(0x04);    //ADRES 1, ADRES2
               liczba1=tmp;
              for(;;){                                  //petla wprowadzania wartosci wyswietlenia
                 send_disp(tablica[liczba2],s_none);              //wwyswietl            
                 if(((PIN_sw & sw_up)==0)){liczba2++; if(liczba2>9)liczba2=0;};            //zwiêksz liczbe
                 if((PIN_sw & sw_down)==0){
                   wpisz_data_i2c(0x04,liczba2);  //ADRES,DARA1,DATA2
                   break;} 
                 if(((PIN_sw & sw_up)==0) && ((PIN_sw & sw_down)==0))break;    //wyjscie z petli
                 OUT_led ^=led_alarm;}   //toogle led
                    }*/
        
         
         //exit
         else if(licz==1){ //wyjscie z case
              flaga_menu=0;   //w³acz wyswietlanie temp.
              OUT_led &=~(led_klapa+led_pompa+led_alarm); //gaœ);
         }
           break;
   } 
   
  //#*************************************************************THE END SWITCH 
    
  
 
    if(((PIN_sw & sw_up)==0) && ((PIN_sw & sw_down)==0)){
        OUT_led =led_alarm;
        flaga_menu=5;
        }     
  }
}  
