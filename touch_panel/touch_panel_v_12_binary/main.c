//******************************************************************************
// Wykona³: Mariusz Piotr Lasota , murphy5@o2.pl
//******************************************************************************



#include  <msp430x12x.h>
#include  "panel.h"
#include  "wait.h"
#include  "i2c.h"

#define t1 1000    //delay
#define test_port P1OUT
#define test_led  BIT7+BIT6+BIT5+BIT4
#define led_A BIT7+BIT6+BIT5+BIT4
#define IRQ_touch BIT1

#define  dana1  P3OUT

volatile unsigned char licz=1;

//------------------------------------------------------------------------------
//                               main
//------------------------------------------------------------------------------
void main(void)
{

  init();   //peryferia
  init_i2c(); //i2c

  /*
8421 bity
0000	-
0001	-sw1
0010	-sw2
0011	-sw3
0100	-sw4
0101	-sw5
0110	-sw6
0111	-sw7
1000	-sw8
1001	-sw9
1010	-sw10
1011	-sw11
1100	-sw12
1101	-
1110	-
1111	-
*/
  
  while(1)
  {  
    
    switch(licz)
    {
      case 1:
        set1_E1();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 =  test_led+BIT0;} // jezeli wciniàto klawisz S2
        wait(t1);                                 //zwiêksz licznik
        break;
        
      case 2:
        set1_E2();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 = test_led+BIT1;}  // jezeli wciniàto klawisz S2
        wait(t1);  
        break;
        
      case 3:
        set1_E3();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 = test_led+BIT1+BIT0;}  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 4:
        set1_E4();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 = test_led+BIT2;}  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 5:
        set1_E5();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 =  test_led+BIT0+BIT2;}      // jezeli wciniàto klawisz S2
        wait(t1);   
        break;
        
      case 6:
        set1_E6();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 =  test_led+BIT2+BIT1;   }  // jezeli wciniàto klawisz S2
        wait(t1);   
        break;
        
      case 7:
        set1_E7();                              //wybierz segment
        if((P2IN&BIT1)!=0x00) {dana1 =  test_led+BIT0+BIT1+BIT2;  }  // jezeli wciniàto klawisz S2
        wait(t1);  
        break;
         } 
      
    switch(licz)
      {
      case 8:
        set2_E1();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1 =  test_led+BIT3;  }  // jezeli wciniàto klawisz S2
        wait(t1);  
        break;
        
      case 9:
        set2_E2();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1= test_led+BIT3+BIT0;  }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 10:
        set2_E3();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1 = test_led+BIT3+BIT1; }  // jezeli wciniàto klawisz S2
        wait(t1);
        break;
        
      case 11:
        set2_E4();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1 =  test_led+BIT3+BIT0+BIT1;  }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 12:
        set2_E5();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1 = test_led+BIT3+BIT2; }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 13:
        set2_E6();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1 = test_led+BIT3+BIT2+BIT1;  }  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
        
      case 14:
        set2_E7();                              //wybierz segment
        if((P2IN&BIT0)!=0x00) {dana1 =  test_led+BIT0+BIT1+BIT2+BIT3;}  // jezeli wciniàto klawisz S2
        wait(t1); 
        break;
    }
    
    
     licz++;
    dana1=0;
        if(licz>14)
        {licz=1;}  
    
  }
}