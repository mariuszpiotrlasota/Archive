#include  "1wire.h"


/**************************************************************************************************************************
procedura:
obsluga 1wire		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void OutWire(unsigned char channel){
  switch(channel){
  case 1: dir_1wire|=bit_1wire; break;
  case 2: dir_2wire|=bit_2wire; break;
  }
  
    
}//OutWire

void InWire(unsigned char channel){
  switch(channel){
  case 1: dir_1wire &= ~bit_1wire; break;
  case 2: dir_2wire &= ~bit_2wire; break;
  }
    
}//InWire

void SetWire(unsigned char channel){
  switch(channel){
  case 1:  port_1wire|=bit_1wire; break;
  case 2:  port_2wire|=bit_2wire; break;
  }    
}//SetWire

void ClrWire(unsigned char channel){
  switch(channel){
  case 1: port_1wire&=~bit_1wire; break;
  case 2: port_2wire&=~bit_2wire; break;
  }
}//ClrWire



/**************************************************************************************************************************
procedura:
WireInit		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void WireInit(unsigned char channel){
  OutWire(channel);
  SetWire(channel);
}//WireInit






/**************************************************************************************************************************
procedura:
WireReset		: zwraca adres docelowy ramki
**************************************************************************************************************************/
unsigned char WireReset(unsigned char channel){
 unsigned char enable_wire=0; 
  OutWire(channel);    //Pull line low and wait for 480uS
  ClrWire(channel);
  wait_500us();
   
  InWire(channel);
  
  switch(channel){
  case 1: if(pin_1wire & bit_1wire) {enable_wire=1;} break;
  case 2: if(pin_2wire & bit_2wire) {enable_wire=1;} break;
  }
  
  return enable_wire;

}//WireReset




/**************************************************************************************************************************
procedura:
Read		: zwraca adres docelowy ramki
**************************************************************************************************************************/
unsigned char WireReadBit(void){
  unsigned char rx_bit=0;
  OutWire(1);
  
  ClrWire(1); 
  wait_10us(); 
  SetWire(1);
  wait_10us();
    
  InWire(1);
    
   if((pin_1wire & bit_1wire)==1) {rx_bit=1;} else{rx_bit=0;}
  
  
  
  return(rx_bit); // return value of DQ line
}


unsigned char WireReadByte(void)
  unsigned char i=8;
  unsigned char rx_buffor = 0;
  
  for(i=0;i<8;i++)
  {
    if(WireReadBit(1)) rx_buffor|=0x01<<i;
     wait_40us(); 
  }
  

  return(rx_buffor);

}//WireRead



/**************************************************************************************************************************
procedura:
WireWrite		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void WireWriteBit(char tx_bit,unsigned char channel){
  ClrWire(channel); 
  if(tx_bit==1){SetWire(channel);}  //1
  else {ClrWire(channel);}       //0
   wait_40us();
   SetWire(channel); 
}// Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us





void WireWriteByte(char tx_byte,unsigned char channel){
   unsigned char i;
   unsigned char pom;
      OutWire(channel); 

  for(i=0;i<8;i++)
  {
    pom=tx_byte>>i;
    pom &= 0x01;
    WireWriteBit(pom,channel);
  }
  SetWire(channel);

}//WireWrite

