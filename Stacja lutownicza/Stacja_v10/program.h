#ifndef program_h
#define program_h



#include "lcd_4b.h"
#include "system.h"
#include "delay.h"
#include "1wire.h"
#include  <msp430x13x.h>

#define S1IN  (P2IN&BIT6)
#define S2IN  (P2IN&BIT7)



#define OutDrive1  BIT7
#define OutDrive2  BIT6
#define OutDrive3  BIT5
#define OutDrive4  BIT4
#define OutDrive5  BIT3
#define OutDriveDIR   P4DIR
#define OutDriveOUT   P4OUT



#define PCF8593_godz 0x04
#define PCF8593_min 0x03
#define PCF8593_sek 0x02



#define PCF8593_reg_kotla 0x0A
#define PCF8593_reg_wody 0x0B

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE 
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
void         set_main_text(void);
void         setTempChanOnLCD(unsigned char channel);
void         SetKotlaWentylatorOFF(void);
void         SetKotlaWentylatorON(void);
void         SetPompaOFF(void);
void         SetPompaON(void);
void         RegulacjaWody(void);
void         RegulacjaKotla(void);
void         RegulacjaPrzedmuch(void);

void         StanPRACA(void);
void         StanROZPAL(void);
void         StanWYGASZ(void);

void         setTemRegKotlaOnLCD(void);
void         setTemRegWodyOnLCD(void);


void         ShowTimeOnLCD(void);


void         SetTimeGodzOnLCD(void);
void         SetTimeMinOnLCD(void);
void         StoreDataOnPCF8593(void);
void         SetPrzedmuch(void);
void         WriteFlashMSP430 (unsigned char Tkotla_regu, unsigned char Twody_regu, unsigned char Przedmuch, unsigned char Wygaszanie);




unsigned char         ReadFlashMSP430_sector(unsigned char sector);

void         WriteFlashMSP430Data(void);
void         ReadFlashMSP430(void);
void         ShowLogoOnLCD(void);
void         initDrivePort(void);
void         setTemWygaszKotlaOnLCD(void);
void         DrivePortOFF(void);
#endif





