
#include  <msp430x12x2.h>

#define BIT_SDA BIT1
#define SDA     1
#define SCL     0
#define I2CDir P1DIR
#define I2COut P1OUT
#define I2CIn  P1IN

#define SET_SDA I2COut |= (1 << SDA)
#define CLR_SDA I2COut &= ~(1 << SDA)
//
#define SET_SCL I2COut |= (1 << SCL)
#define CLR_SCL I2COut &= ~(1 << SCL)
//
#define SDA_OUT I2CDir |= (1 << SDA)
#define SDA_IN  I2CDir &= ~(1 << SDA)
//
#define SCL_OUT I2CDir |= (1 << SCL)
#define SCL_IN  I2CDir &= ~(1 << SDA)
//
#define GET_SDA (I2CIn & (1 << SDA))
//
#define ACK 0
#define NOACK 1

//zmienne globalne
unsigned char tmp,tmp2;  





void wait(void){
    volatile unsigned int i;
    i = 2;                              // Delay
    do (i--);
    while (i != 0);
}


void delay(void){
    volatile unsigned int i;
    i = 50;                              // Delay
    do (i--);
    while (i != 0);
}


// funkcja generujaca sygna� start
void i2cstart(void){
  SDA_OUT; //
  SCL_OUT; // ustawienie linii SDA i SCL w tryb wyj�ciowy
  SET_SDA; //
  SET_SCL; // ustawienie na liniach SDA i SCL stanu wysokiego
  delay(); // op�nienie
  CLR_SDA;
  delay();
  CLR_SCL;
}

// funkcja generujaca sygna� stop
void i2cstop(void){
  CLR_SDA;
  delay();
  SET_SCL;
  delay();
  SET_SDA;
  delay();
}


// funkcja wysy�aj�ca bajt na szyn� I2C
void i2cwrite(unsigned char x){
  unsigned char count = 9;
  do
  {
  CLR_SCL;
  if(x & 0x80) SET_SDA; else CLR_SDA;
  x <<= 1;
  delay();
  SET_SCL;
  delay();
  }while(--count);
  CLR_SCL;
}




// funkcja odczytujaca bajt z szyny I2C
unsigned char i2cread(void){
  unsigned char count = 8, temp = 0;
  SET_SDA; 
  SDA_IN;
  do{
    delay();
    SET_SCL;
    delay();
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

void wpisz_data_i2c(char adres, char dana1 ,char dana2){
      //wpisywanie warto�ci 
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres); 

        i2cwrite(dana1);        //0X00
        i2cwrite(dana2);        //0X00
        i2cstop();
}


void czytaj_data_i2c(char adres1, char adres2){
        //wpisywanie warto�ci 
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres1); 
        i2cstop();   
        i2cstart();
        i2cwrite(0xA1);     //wpisany adres pamieci na magistrali 
//------warto�� 1
        tmp = i2cread();
        wait();
        i2cstop();
        
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres2); 
        i2cstop();   
        i2cstart();
        i2cwrite(0xA1);     //wpisany adres pamieci na magistrali 
//------warto�� 1
        tmp2 = i2cread();
        wait();
        i2cstop();       
        
        
}

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  

  I2CDir = BIT0+BIT1;

 //wpisz_data_i2c(0x00,0x5,0x66);
 czytaj_data_i2c(0x00,0x01);
 
 P2DIR= BIT2|BIT3;
 P2OUT=0X00; 
  for (;;)
  { 
    if(tmp==0x05)
    {
     P2OUT ^= BIT3;
    }
     else
    {
     P2OUT ^= BIT2;
    }
  

  }
}
