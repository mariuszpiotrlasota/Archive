#include  "1wire.h"


/**************************************************************************************************************************
procedura:
obsluga 1wire		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void OutWire(void){
    dir_1wire|=bit_1wire;
    //dir_test|=bit_test;
}//OutWire

void InWire(void){
    dir_1wire &= ~bit_1wire;
}//InWire

void SetWire(void){
    port_1wire|=bit_1wire; // '1'
    //port_test|=bit_test; // '1'
}//SetWire

void ClrWire(void){
    port_1wire&=~bit_1wire; // '0'
    //port_test &=~bit_test; // '0'
}//ClrWire



/**************************************************************************************************************************
procedura:
WireInit		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void WireInit(void){
  OutWire();
  SetWire();
}//WireInit






/**************************************************************************************************************************
procedura:
WireReset		: zwraca adres docelowy ramki
**************************************************************************************************************************/
unsigned char WireReset(void){
 unsigned char enable_1wire=0; 
  OutWire();    //Pull line low and wait for 480uS
  ClrWire();
  wait_500us();
   
  InWire();
                          //Release line and wait for 60uS
  if(pin_1wire & bit_1wire) {enable_1wire=1;} //Store line value and wait until the completion of 480uS period
  return enable_1wire;

}//WireReset




/**************************************************************************************************************************
procedura:
Read		: zwraca adres docelowy ramki
**************************************************************************************************************************/
unsigned char WireReadBit(void){
  unsigned char rx_bit=0;
  OutWire();
  
  ClrWire(); 
  wait_10us(); 
  SetWire();
  wait_10us();
    
  InWire();
  if((pin_1wire & bit_1wire)==1) {rx_bit=1;} //Read line value
  else{rx_bit=0;}
  
  return(rx_bit); // return value of DQ line
}


unsigned char WireReadByte(void){
  unsigned char i=8;
  unsigned char rx_buffor = 0;
  
  for(i=0;i<8;i++)
  {
    if(WireReadBit()) rx_buffor|=0x01<<i;
     wait_40us(); 
  }
  
  /*
  while(i--)
  {
      
      rx_buffor>>=1;
      rx_buffor|=(WireReadBit()<<7);
  }*/
  return(rx_buffor);

}//WireRead



/**************************************************************************************************************************
procedura:
WireWrite		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void WireWriteBit(char tx_bit){
  ClrWire(); 
  if(tx_bit==1){SetWire();}  //1
  else {ClrWire();}       //0
   wait_40us();
   SetWire(); 
}// Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us


void WireWriteByte(char tx_byte){
   unsigned char i;
   unsigned char pom;
      OutWire(); 

  for(i=0;i<8;i++)
  {
    pom=tx_byte>>i;
    pom &= 0x01;
    WireWriteBit(pom);
  }
  SetWire();

}//WireWrite

