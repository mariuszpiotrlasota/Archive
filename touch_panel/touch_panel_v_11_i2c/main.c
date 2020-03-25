//******************************************************************************
// Wykona³: Mariusz Piotr Lasota , murphy5@o2.pl
//******************************************************************************



#include  <msp430x12x.h>
#include  "panel.h"
#include  "wait.h"
#include  "i2c.h"

#define t1 500    //delay
#define test_port P1OUT
#define test_led  BIT
#define led_A BIT7+BIT6+BIT5+BIT4
#define IRQ_touch BIT1


volatile unsigned char dana1=0;
volatile unsigned char dana2=0;
volatile unsigned char licz=1;





//------------------------------------------------------------------------------
//                               main
//------------------------------------------------------------------------------
void main(void)
{

  init();   //peryferia
  init_i2c(); //i2c

  while(1)
  {  
    
    switch(licz)
    {
      case 1:
        set1_E1();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A;  dana1 |= BIT0+BIT1;} // jezeli wciniàto klawisz S2
        wait(t1);                                 //zwiêksz licznik
        break;
        
      case 2:
        set1_E2();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A; dana1 |= BIT0+BIT2;}  // jezeli wciniàto klawisz S2
        wait(t1);  
        break;
        
      case 3:
        set1_E3();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A; dana1 |= BIT0+BIT3;}  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 4:
        set1_E4();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A; dana1 |= BIT0+BIT4;}  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 5:
        set1_E5();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A; dana1 |= BIT0+BIT5;}      // jezeli wciniàto klawisz S2
        wait(t1);   
        break;
        
      case 6:
        set1_E6();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A; dana1 |= BIT0+BIT6;   }  // jezeli wciniàto klawisz S2
        wait(t1);   
        break;
        
      case 7:
        set1_E7();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {P3OUT = led_A;  dana1 |= BIT0+BIT7;  }  // jezeli wciniàto klawisz S2
        wait(t1);  
        break;
         } 
    
    
    switch(licz)
      {
      case 8:
        set2_E1();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT1;       }  // jezeli wciniàto klawisz S2
        wait(t1);  
        break;
        
      case 9:
        set2_E2();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT2;   }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 10:
        set2_E3();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT3; }  // jezeli wciniàto klawisz S2
        wait(t1);
        break;
        
      case 11:
        set2_E4();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT4;  }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 12:
        set2_E5();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT5; }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 13:
        set2_E6();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT6;  }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 14:
        set2_E7();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {P3OUT = led_A; dana2 |= BIT0+BIT7; }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;

    }
     licz++;
  
     if(licz>14){
       licz=1;
       if((dana1&BIT0)==1) {P3OUT |=IRQ_touch;  wpisz_data_i2c(0x01,dana1);}   //wyslij dan¹
        P3OUT &= IRQ_touch;     //czysc przerwanie
       if((dana2&BIT0)==1) {P3OUT |=IRQ_touch;  wpisz_data_i2c(0x02,dana2);}   //wyslij dan¹
        P3OUT &= IRQ_touch;     //czysc przerwanie
       dana1=0x00;      //czyœæ bufory
       dana2=0x00;
     }
     
     

       P3OUT &= ~0XF0; 
  }
}