#include  "1wire.h"


/**************************************************************************************************************************
procedura:
obsluga 1wire		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void OutWireChan(unsigned char channel){
  switch(channel){
  case 1: dir_1wire|=bit_1wire; break;
  case 2: dir_2wire|=bit_2wire; break;
  case 3: dir_3wire|=bit_3wire; break;
  case 4: dir_4wire|=bit_4wire; break;
  }
  
    
}//OutWire

void InWireChan(unsigned char channel){
  switch(channel){
  case 1: dir_1wire &= ~bit_1wire; break;
  case 2: dir_2wire &= ~bit_2wire; break;
  case 3: dir_3wire &= ~bit_3wire; break;
  case 4: dir_4wire &= ~bit_4wire; break;
  }
    
}//InWire

void SetWireChan(unsigned char channel){
  switch(channel){
  case 1:  port_1wire|=bit_1wire; break;
  case 2:  port_2wire|=bit_2wire; break;
  case 3:  port_3wire|=bit_3wire; break;
  case 4:  port_4wire|=bit_4wire; break;
  }    
}//SetWire

void ClrWireChan(unsigned char channel){
  switch(channel){
  case 1: port_1wire&=~bit_1wire; break;
  case 2: port_2wire&=~bit_2wire; break;
  case 3: port_3wire&=~bit_3wire; break;
  case 4: port_4wire&=~bit_4wire; break;
  }
}//ClrWire


/**************************************************************************************************************************
procedura:
WireInit		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void WireInit(unsigned char channel){
  OutWireChan(channel);
  SetWireChan(channel);
}//WireInit






/**************************************************************************************************************************
procedura:
WireReset		: zwraca adres docelowy ramki
**************************************************************************************************************************/
unsigned char WireReset(unsigned char channel){
 unsigned char enable_wire=0; 
  OutWireChan(channel);
  ClrWireChan(channel);
  wait_500us();
   
  InWireChan(channel);
  
  switch(channel){
  case 1: if(pin_1wire & bit_1wire) {enable_wire=1;} break;
  case 2: if(pin_2wire & bit_2wire) {enable_wire=1;} break;
  case 3: if(pin_3wire & bit_3wire) {enable_wire=1;} break;
  case 4: if(pin_4wire & bit_4wire) {enable_wire=1;} break;
  }

  return enable_wire;

}//WireReset




/**************************************************************************************************************************
procedura:
Read		: zwraca adres docelowy ramki
**************************************************************************************************************************/
unsigned char WireReadBit(unsigned char channel){
  unsigned char rx_bit=0;
  OutWireChan(channel);
  
  ClrWireChan(channel);
  //wait_10us(); 
  SetWireChan(channel);
  //wait_10us();
    
  InWireChan(channel);
  switch(channel){
    case 1: if((pin_1wire & bit_1wire)!=0) {rx_bit=1;}  else{rx_bit=0;} break;
    case 2: if((pin_2wire & bit_2wire)!=0) {rx_bit=1;}  else{rx_bit=0;} break;
    case 3: if((pin_3wire & bit_3wire)!=0) {rx_bit=1;}  else{rx_bit=0;} break;
    case 4: if((pin_4wire & bit_4wire)!=0) {rx_bit=1;}  else{rx_bit=0;} break;
  }
  
  return(rx_bit); // return value of DQ line
}


unsigned char WireReadByte(unsigned char channel){
  unsigned char i=8;
  unsigned char rx_buffor = 0;
  
  for(i=0;i<8;i++)
  {
    if(WireReadBit(channel)) rx_buffor|=0x01<<i;
     wait_40us(); 
  }
  
  return(rx_buffor);

}//WireRead




/**************************************************************************************************************************
procedura:
WireWrite		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void WireWriteBit(char tx_bit,unsigned char channel){
  ClrWireChan(channel);
  if(tx_bit==1){SetWireChan(channel);}  //1
  else {ClrWireChan(channel);}       //0
   wait_40us();
   SetWireChan(channel);
}// Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us


void WireWriteByte(char tx_byte,unsigned char channel){
   unsigned char i;
   unsigned char pom;
      OutWireChan(channel);

  for(i=0;i<8;i++)
  {
    pom=tx_byte>>i;
    pom &= 0x01;
    WireWriteBit(pom,channel);
  }
  SetWireChan(channel);

}//WireWrite

