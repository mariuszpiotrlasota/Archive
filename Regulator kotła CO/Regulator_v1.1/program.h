#ifndef _program_h
#define _program_h

#include  <msp430x12x2.h>
#include  "1wire.h"


#define SkipROM 0xCC
#define StartConversion 0x44
#define ReadScratchPad 0xBE 


#define Yes   1
#define No    0


/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE 1wire
-------------------------------------------------------------------------------------------------------------------------*/
#define THERM_CMD_CONVERTTEMP  0x44
#define THERM_CMD_RSCRATCHPAD   0xbe
#define THERM_CMD_WSCRATCHPAD  0x4e
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xb8
#define THERM_CMD_RPWRSUPPLY 0xb4
#define THERM_CMD_SEARCHROM 0xf0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xcc
#define THERM_CMD_ALARMSEARCH 0xec
#define THERM_DECIMAL_STEPS_12BIT 625


/*
Dallas ds18b20 reg.
SEARCH ROM        [F0h] 
READ ROM          [33h] 
Convert T         [44h] -This command initiates a single temperature conversion
MATCH ROM         [55h]
SKIP ROM          [CCh] 
Read Scratchpad   [BEh]
ALARM SEARCH      [ECh] 
WRITE SCRATCHPAD  [4Eh] 
READ SCRATCHPAD   [BEh] 
COPY SCRATCHPAD   [48h] 
RECALL E2         [B8h] 


PROCES:
a) Inicjalizacja 
    Jest to sygna³ esetu magistrali wykonywany przez mikrokontroler steruj¹cy. 
    Ka¿da transmisja przez magistralê 1-Wire musi rozpocz¹æ siê od tego sygna³u.

b)Komendy ROM 
    S¹ to polecenia s³u¿¹ce do operowania na unikalnym 64-bitowym kodzie uk³adu. 
    Zapewniaj¹ okreœlenie ile i jakie uk³ady podpiête s¹ do magistrali, 
    odczytanie numerów zapisanych w ich pamiêci ROM oraz uaktywnienie uk³adu 
    o znanym wczeœniej numerze.
    Do najwa¿niejszych poleceñ z tej grupy nale¿¹:
  Reed Rom [33h] - Pozwala procesorowi na odczyt 8 bajtów numeru seryjnego uk³adu. 
    Nie dzia³a gdy na magistrali jest wiêcej ni¿ jeden uk³ad.
  Match Rom [55h] - Umozliwia aktywowanie jednego z uk³adów na magistrali za 
    pomoc¹ jego 64-bitowego numeru.
  Skip Rom [CCh] - Pozwala na pominiêcie sprawdzania numerów uk³adów i wys³anie 
    jednoczeœnie rozkazów do wszystkich uk³adów na magistrali. Komenda jest 
    przydatna do wysy³ania poleceñ konwersji temperatury do wszystkich czujników 
    temperatury pracuj¹cych na jednej magistrali, lub gdy u¿ywamy tylko jednego 
    uk³adu DS18B20.

c) Komendy Funkcji Uk³adu
    Rozkazy te nastêpuj¹ po komendach dotycz¹cych wyboru uk³adu lub pominiêcia 
    sprawdzania numeru. S³u¿¹ do konwersji temperatury oraz do zapisu lub odczytu 
    danych z rejestrów DS18B20. Do najwa¿niejszych poleceñ z tej grupy nale¿¹:
  Convert T [44h] - Rozkaz ten rozpoczyna proces konwersji temperatury, 
    po jego zakoñczeniu wyniki bêd¹ umieszczone w dwóch bajtach rejestru DS18B20
  Read Scratchpad [BEh] - Rozkaz powoduje sekwencjê odczytu danych z rejestru 
    czujnika pocz¹wszy od bajtu 0 do 8. Odczyt mo¿e byæ przerwany przez sygna³ 
    resetu magistrali 1-Wire je¿eli nie potrzeba wszystkich 9 bajtów.
*/

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
void          program(void);
void          ReadTemperature(void);
#endif
