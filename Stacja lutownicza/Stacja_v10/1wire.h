#ifndef _1wire_h
#define _1wire_h

#include  <msp430x13x.h>
#include  "delay.h"

#define Yes   1
#define No    0

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE 1wire
-------------------------------------------------------------------------------------------------------------------------*/

#define dir_1wire   P4DIR
#define port_1wire  P4OUT
#define bit_1wire   BIT0
#define pin_1wire   P4IN
//4,0 1wire

#define dir_2wire   P4DIR
#define port_2wire  P4OUT
#define bit_2wire   BIT1
#define pin_2wire   P4IN
//4,1 1wire

#define dir_3wire   P4DIR
#define port_3wire  P4OUT
#define bit_3wire   BIT2
#define pin_3wire   P4IN
//4,2 1wire

#define dir_4wire   P4DIR
#define port_4wire  P4OUT
#define bit_4wire   BIT3
#define pin_4wire   P4IN
//4,3 1wire


/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE STRUKTUR
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE ZMIENNYCH
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE ZMIENNYCH WSPOLDZIELONYCH
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
			DEKLARACJE PROCEDUR
-------------------------------------------------------------------------------------------------------------------------*/

void OutWireChan(unsigned char channel);

void InWireChan(unsigned char channel);

void SetWireChan(unsigned char channel);

void ClrWireChan(unsigned char channel);


void WireInit(unsigned char channel);


unsigned char WireReset(unsigned char channel);

unsigned char WireReadBit(unsigned char channel);


unsigned char WireReadByte(unsigned char channel);

void WireWriteBit(char tx_bit,unsigned char channel);

void WireWriteByte(char tx_byte,unsigned char channel);





#endif
