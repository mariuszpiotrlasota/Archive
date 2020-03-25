#include  "program.h"


/*
Procedury DS'a
1.RESET
2.PRZESKOCZ ROM 
3.ZMIERZ TEMPERATURE
4.UTRZYMAJ STAN '1'
5.RESET
6.PRZESKOCZ ROM 
7.ODCZYT TEMPERATURY 
8.ODBIERAJ DANE OD DS'a
*/

/**************************************************************************************************************************
procedura:
program 	: zwraca adres docelowy ramki
**************************************************************************************************************************/
void program(void){
    P1OUT ^= BIT2+BIT3;                          // Toggle P1.0 using exclusive-OR
}






/**************************************************************************************************************************
procedura:
program 	: zwraca adres docelowy ramki
**************************************************************************************************************************/
void ReadTemperature(void){
  unsigned char  temperature[2];
  unsigned char digit;
  unsigned int decimal;  
  
  WireReset();
  WireWriteByte(THERM_CMD_SKIPROM);
  WireWriteByte(THERM_CMD_CONVERTTEMP);
  while(!WireReadBit());
  WireReset();  
  WireWriteByte(THERM_CMD_SKIPROM);
  WireWriteByte(THERM_CMD_RSCRATCHPAD);
  
  temperature[0]=WireReadByte();
  temperature[1]=WireReadByte();
  WireReset();
  
  //Store temperature integer digits and decimal digits
  digit=temperature[0]>>4;
  digit|=(temperature[1]&0x7)<<4;
  //Store decimal digits
  decimal=temperature[0]&0xf;
  decimal*=THERM_DECIMAL_STEPS_12BIT;
  

  
}

