
#include  <msp430x16x.h>
#include  "wait.h"
#include  "swiatla.h"
#include  "motor.h"
#include  "i2c.h"

#define motor_led (BIT4)  //p4.4
#define PORT_motor_led  P4OUT

#define IRQ_touch BIT1
#define IRQ_DIR P1DIR
#define IRQ_PIN P1IN
/*
PIN  
P1.1 - IRQ
P1.2 - SDA
P1.3 - SCL
*/

unsigned char bufor_rx_1=0;
unsigned char bufor_rx_2=0;
unsigned char bufor_rx_3=0;




//------------------------------------------------------------------------------
//                              init
//------------------------------------------------------------------------------
void init_io(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= 0xFF;                            // Set P1.0 to output direction
  IRQ_DIR &= (~IRQ_touch);                   //wejscie IRQ panela 
  P2DIR |= 0XFF;
  P3DIR |= 0XFF;
  P4DIR |= 0XFF;
  P5DIR |= 0XFF;
  P6DIR |= 0XFF;
  P5OUT = 0X00;
}

void led_start_axis(void){
    volatile unsigned int ile=0;
    led_test1();
    led_test2();
    for(ile;ile<5;ile++){ led_blask3();} ile=0;
}
 

//------------------------------------------------------------------------------
//                                funkcje panelu 
//------------------------------------------------------------------------------
void touch_panel(void){
  
    if((IRQ_PIN&IRQ_touch)==0x00)
    {
      while((IRQ_PIN&IRQ_touch)==0){};
      bufor_rx_2=i2cTouchReadAdres();    
      bufor_rx_1=i2cTouchReadChannel();      //odczytaj dan¹
      
          //bufor_rx_3=i2cread_axis();
          
         if(bufor_rx_2==0x0A){
           PORT_motor_led |= motor_led;
            wait_short(); 
            PORT_motor_led &= ~motor_led;
             wait_short(); 
          }
          /*
          if(bufor_rx_2==0xA0){
            PORT_motor_led |= motor_led;
            wait_short(); 
            PORT_motor_led &= ~motor_led;
          }
          */
          wait_short(); 
          
    }
}

//------------------------------------------------------------------------------
//                                MAIN
//------------------------------------------------------------------------------


void main(void)
{
  init_io();    //Konfiguracje portow
  init_i2c();   //wlacz I2C
  m_clear();    //czysc porty 
  //test_m();
 // led_start_axis();
  
 
  
   PORT_motor_led &= ~motor_led;

  while(1)
  {   
    touch_panel();
    
  }
}
