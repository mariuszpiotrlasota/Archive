
#include  <msp430x12x.h>
#include  "i2c.h"


//zmienne globalne
unsigned char tmp;  //wartoùci wys-ane/odczytane z pamieci

//******************************************************************************
//             FUNKCJA DLA I2C
//******************************************************************************

void init_i2c(void){
    I2CDir |= BIT_SCL+BIT_SDA;
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
      //wpisywanie wartoùci 
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres); 
        i2cwrite(dana1);        //0X00
        i2cstop();
}


void czytaj_data_i2c(char adres1){
        //wpisywanie wartoùci 
        i2cstart();
        i2cwrite(0xA0);     //wpisany adres pamieci na magistrali
        i2cwrite(adres1); 
        i2cstop();   
        i2cstart();
        i2cwrite(0xA1);     //wpisany adres pamieci na magistrali 
//------wartoùä 1
        tmp = i2cread();
        wait_I2C();
        i2cstop();    
}

        




