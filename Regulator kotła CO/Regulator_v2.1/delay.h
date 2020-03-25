#ifndef _delay_h
#define _delay_h

#include  <msp430x13x.h>


#define Yes   1
#define No    0

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE 1wire
-------------------------------------------------------------------------------------------------------------------------*/

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
void          wait(void);
void          wait_10us(void);
void          wait_40us(void);
void          wait_100us(void);
void          wait_160us(void);
void          wait_500us(void);
void          wait_short(void);
void          wait_long(void);
void          wait_long2(void);
void          DelayNus(unsigned int n);
void          procesorDelay(unsigned long p_ulDelay);
#endif