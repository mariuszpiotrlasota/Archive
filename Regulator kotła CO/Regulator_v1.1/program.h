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
    Jest to sygna� esetu magistrali wykonywany przez mikrokontroler steruj�cy. 
    Ka�da transmisja przez magistral� 1-Wire musi rozpocz�� si� od tego sygna�u.

b)Komendy ROM 
    S� to polecenia s�u��ce do operowania na unikalnym 64-bitowym kodzie uk�adu. 
    Zapewniaj� okre�lenie ile i jakie uk�ady podpi�te s� do magistrali, 
    odczytanie numer�w zapisanych w ich pami�ci ROM oraz uaktywnienie uk�adu 
    o znanym wcze�niej numerze.
    Do najwa�niejszych polece� z tej grupy nale��:
  Reed Rom [33h] - Pozwala procesorowi na odczyt 8 bajt�w numeru seryjnego uk�adu. 
    Nie dzia�a gdy na magistrali jest wi�cej ni� jeden uk�ad.
  Match Rom [55h] - Umozliwia aktywowanie jednego z uk�ad�w na magistrali za 
    pomoc� jego 64-bitowego numeru.
  Skip Rom [CCh] - Pozwala na pomini�cie sprawdzania numer�w uk�ad�w i wys�anie 
    jednocze�nie rozkaz�w do wszystkich uk�ad�w na magistrali. Komenda jest 
    przydatna do wysy�ania polece� konwersji temperatury do wszystkich czujnik�w 
    temperatury pracuj�cych na jednej magistrali, lub gdy u�ywamy tylko jednego 
    uk�adu DS18B20.

c) Komendy Funkcji Uk�adu
    Rozkazy te nast�puj� po komendach dotycz�cych wyboru uk�adu lub pomini�cia 
    sprawdzania numeru. S�u�� do konwersji temperatury oraz do zapisu lub odczytu 
    danych z rejestr�w DS18B20. Do najwa�niejszych polece� z tej grupy nale��:
  Convert T [44h] - Rozkaz ten rozpoczyna proces konwersji temperatury, 
    po jego zako�czeniu wyniki b�d� umieszczone w dw�ch bajtach rejestru DS18B20
  Read Scratchpad [BEh] - Rozkaz powoduje sekwencj� odczytu danych z rejestru 
    czujnika pocz�wszy od bajtu 0 do 8. Odczyt mo�e by� przerwany przez sygna� 
    resetu magistrali 1-Wire je�eli nie potrzeba wszystkich 9 bajt�w.
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
