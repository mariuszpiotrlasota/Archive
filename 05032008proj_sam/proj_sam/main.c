#include <stdio.h>                         
#include <LPC214x.H>                   
#include <string.h>
#include "const_bit.h"
#include "delay.h"
#include "arm1_11_mif7.h"
#define AD0CR_CLKDIV_BIT 8
#define AD0CR_PDN_BIT 21
#define AD0CR_SEL_MASK 0xFF
#define AD0CR_START_BIT 24
#define AD0GDR_DONE 0x80000000

#define ster_enable    IOSET1=BIT16
#define ster_disable   IOCLR1=BIT16
#define jazda_enable   IOSET1=BIT17
#define jazda_disable  IOSET1=BIT17

#define P028_ADC01_SEL (1<<24)
#define P029_ADC02_SEL (1<<26)

void init_timer (void) ;
int get_ad(int kanal);
int get_ad_chanel(int Chan);
void init_ad(void);
void odebranie(void);
void akcelerometr(int k);

void Inicjacja(void);				 //inicjaclja
void Delay_ms(int czas);			//pauzy  ms
void Delay_us(int czas); 			//pauzy  us
unsigned char ModRxStart(void);	    //odczyt danych z modu-u radiowego 
void RadioTransAndWait(unsigned char tx_ile); //transmisja radiowa i oczekiwanie na przes-anie UARTEM przez partnera otrzymanych danych  
void Rf_Power_Up_Reset(void); 		//sekwencja zerowania transceiwera po w-¶czeniu zasilania
void Rf_Send_Packet(unsigned char *txBuffer, unsigned char size);//transmisja pakietu danych  
void RfWriteRfSettings(RF_SETTINGS *pRfSettings);	 	//programowanie rejestr°w CC1100
void SpiReadBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile); //odczyt grupowy zawartoùci rejestr°w 
unsigned char SpiReadReg(unsigned char adres); //odczyt zawartoùci pojedynczego rejestru 
void SpiWriteBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile);  //grupowy zapis do kolejnych rejestr°w  
void SpiWriteCommand(unsigned char adres); //wys-anie pojedynczego rozkazu 
void SpiWriteReg(unsigned char adres, unsigned char wartosc); //zapis wartoùci do pojedynczego rejestru
void Test(void); 					 //test toru 
void UART0_Init(void);				 //inicjacja UART0
void UartRxStart(void);	 			 //odczyt danych z UARTU    
void WyrownanieWskaznikowBufora(void);	//wyr°wnanie wskaËnik°w dost‡pu do bufora


 
//*****************************************************************odczyt danych z modu-u radiowego 
unsigned char ModRxStart(void)	 //wy:	iloùä odebranych danych, 0- prawdopodobnie nast¶pi- b-¶d w czasie odbioru pakietu

{  
	unsigned char status[2];
	static unsigned char packed_length; 
	unsigned char x;
  	 	
    IO0SET = IO0SET | DSR_SIGNAL;//wstrzymanie odbioru nowych danych portem RS   
	IO0SET = IO0SET | LED_REC_PORT; 
	WyrownanieWskaznikowBufora();//wyr°wnanie wskaËnik°w dost‡pu do bufora    
	// Wait for GDO0 to be cleared -> end of packet 
	// oczekiwanie na stan niski wyjùcia DGO0 sygnalizuj¶cy odbi°r kompletnego pakietu danych
 	while ((IO0PIN & GDO_RF_PIN) !=0); 
	  	SpiReadBurstReg(CCxxx0_RXBYTES, &packed_length, 1);   //odczyt rejestru statusu
	  	packed_length =packed_length & BYTES_IN_RXFIFO;
	  	if (packed_length !=0)//odczytano co najmniej 1 bajt
	  	{          
	   		//odczyt 1 przeslanego bajtu zawieraj¶cego d-ugoùä przeslanego pakietu danych
	   		packed_length =SpiReadReg(CCxxx0_RXFIFO);
	   		if (packed_length <=BUFOR_SIZE)
	   		{
	   			SpiReadBurstReg(CCxxx0_RXFIFO, &bufor[0], packed_length);    
	   			//odczyt 2 bajt°w statusu: status[0]=RSSI, status[1] =LQI
	   			SpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 
	   			if ((status[LQI] &CRC_OK) ==0) packed_length =0;//b-¶d sumy CRC przeslanego pakietu
			} 
	   		else
	   		{  
	   			//b-¶d, przes-any pakiet > od rozmiaru buforu  Flush RX FIFO
	      		SpiWriteCommand(CCxxx0_SFRX);
	      		packed_length =0; 	
	   		}
	  	} 
//**********************
	 odebranie(); 
	SpiWriteCommand(CCxxx0_SFRX);  
  	SpiWriteCommand(CCxxx0_SIDLE); 
  	Delay_ms(1); 
  	SpiWriteCommand(CCxxx0_SRX);    
   	IO0CLR = IO0CLR | LED_REC_PORT;  
	while(1)
	{	
		//sprawdzenie czy odebrana transmisja jest transmisja testowa
		if (packed_length !=sizeof(test_txt_eep)-1) break;
		for (x=0; x<sizeof(test_txt_eep)-1; x++)
		{
			if (x ==0) continue;//nie kontrolowane znaki w transmisji
			if (bufor[x] !=test_txt_eep[x]) break;
		}            
		if (x !=(sizeof(test_txt_eep)-1)) break;
 
 	  
    	if (bufor[0] !='T')//przeslanie w trybie testowym    
    	{    
    		//odebrana transmisja testowa by-a odpowiedzi¶, 
    		//wysy-anie odpowiedzi na odpowiedË jest zabronione
      	//Delay_ms(200); //pauza 200ms
      	break;
    	}
 
    	//wysy-anie odpowiedzi na transmisj‡ testow¶ partnera   
    	Delay_ms(10); //pauza 10ms
    	request_trans_flag =1;  
    	WyrownanieWskaznikowBufora();  
    	Test();   
  		SpiWriteCommand(CCxxx0_SIDLE);  
		Delay_ms(1);
  		SpiWriteCommand(CCxxx0_SRX);  
    
    	break;
  }   
  IO0CLR = IO0CLR | DSR_SIGNAL;//odblokowanie transmisji nowych danych portem UART-a   
  return packed_length;
}
//****************************************************


 //*****************************************************************procedura pauzy w ms
void Delay_ms(int czas)
{
	#define PETLA_1MS		6006   //10006
	czas =czas *PETLA_1MS;
	while (czas !=0) czas--;
}

void Delay_us(int czas)
{
	#define PETLA_1US	1
	czas =czas *PETLA_1US;
	while (czas !=0) czas--;
}

//***********************************************************procedury inicjacji
void Inicjacja(void)
{
	VPBDIV = 0x02;	//Divide Pclk by two 30MHz
  	IODIR1 = 0x00FFF000;
	rx_start_rec_flag =0; 	//zerowanie kluczowych flag  
	uart_rxd_flag =0;      

	IO0DIR = IO0DIR | CS_RF_PORT |SI_RF_PORT | SCLK_RF_PORT ; //pin P0.2, P0.3, P0.4, 
	IO0DIR |= IO0DIR | LED_TRANS_PORT | LED_REC_PORT | LED_ERROR_PORT |DSR_SIGNAL; //P0.16, P0.17, P0.18, P0.24 
	IO0SET = IO0SET | CS_RF_PORT;  //CS=1		   //configured as output
	IO0CLR =IO0CLR | DSR_SIGNAL;  //DSR=0
	IO0SET = IO0SET | LED_TRANS_PORT;//LED_tx=1	   		//RESET start 	
    IO0SET = IO0SET | LED_REC_PORT;  	 
	Delay_ms(100); //pauza 100ms
	Rf_Power_Up_Reset(); 
	RfWriteRfSettings(&rfSettings);
	SpiWriteBurstReg(CCxxx0_PATABLE, &paTable[0], sizeof(paTable));
 	IO0CLR = IO0CLR | LED_TRANS_PORT;//LED on
    IO0CLR = IO0CLR | LED_REC_PORT; 
}

//*******************************transmisja radiowa i oczekiwanie na przes-anie UARTEM przez partnera odebranych danych  
void RadioTransAndWait(unsigned char tx_ile)
//we:	tx_ile -ile bajt°w do wys-ania  
//		nr_uart -numer szybkoùci UARTA
{  
float czas_transmisji_uartem;
unsigned int pauza;
     

	memmove(&bufor[1], &bufor[0], tx_ile);    
	//wpisanie na 1 pozycj‡ buforu iloùci transmitowanych danych w pakiecie
	bufor[0] =tx_ile;
	//transmisja danych torem radiowym + 1 bajt d-ugoùci transmisji
	Rf_Send_Packet(&bufor[0], tx_ile+1);
  	//maksymalny czas oczekiwania na wys-anie przez partnera UARTEM
  	//odebrnych danych =(iloùä danych+1 * czas transmisji UARTEM 1 ramki) 
  	czas_transmisji_uartem =((float)(tx_ile +5) *TRANSMISJA_1_RAMKI_9600);
  	pauza =(unsigned int)(czas_transmisji_uartem);
  	Delay_ms(pauza);
}


//************************************************sekwencja zerowania transceiwera po w-¶czeniu zasilania
void Rf_Power_Up_Reset(void)
{ 
unsigned char maska, x;

	IO0SET = IO0SET | SCLK_RF_PORT;//CLK=1
	IO0CLR =IO0CLR | SI_RF_PORT;   //SI =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	Delay_us(2);
	IO0SET =IO0SET | CS_RF_PORT;   //CS =1 
	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	Delay_us(40); //pauza 40us
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 
     
	maska =0x80;
	//wys-anie przez SPI komendy CCxxx0_SRES
	for (x=0; x<8; x++)
	{ 
		if ((CCxxx0_SRES &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;
		Delay_us(1);	
	}
	
	while ((IO0PIN & SO_RF_PIN) ==0);//oczekiwanie na stan wysoki linii SO  
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na ponowny stan niski linii SO    
	IO0SET =IO0SET | CS_RF_PORT;   //CS =1    
}
//******************************************************transmisja pakietu danych  
void Rf_Send_Packet(unsigned char *txBuffer, unsigned char size)
{               
	SpiWriteCommand(CCxxx0_SIDLE); 
	SpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size); 
	SpiWriteBurstReg(CCxxx0_PATABLE, &paTable[0], sizeof(paTable));
	SpiWriteCommand(CCxxx0_STX);	 
	
	while ((IO0PIN & GDO_RF_PIN) ==0); //oczekiwanie na stan wysoki na wyjùciu GDO -> sync transmitted 

	IO0SET = IO0SET | LED_TRANS_PORT;//LED on	   	//IO0CLR = IO0CLR | LED_TRANS_PORT;//LED on

	while ((IO0PIN & GDO_RF_PIN) !=0);	 	//oczekiwanie na stan niski wyjùcia GDO -> end of packet

	IO0CLR = IO0CLR | LED_TRANS_PORT;//LED off	 	//IO0SET = IO0SET | LED_TRANS_PORT;//LED off

	SpiWriteCommand(CCxxx0_SIDLE); 
	Delay_ms(1); 
    SpiWriteCommand(CCxxx0_SRX);
	Delay_ms(1);
}


//**************************************************programowanie rejestr°w CC1100
void RfWriteRfSettings(RF_SETTINGS *pRfSettings) 
{
    // Write register settings
    SpiWriteReg(CCxxx0_FSCTRL1,  pRfSettings->FSCTRL1);
    SpiWriteReg(CCxxx0_FSCTRL0,  pRfSettings->FSCTRL0);
    SpiWriteReg(CCxxx0_FREQ2,    pRfSettings->FREQ2);
    SpiWriteReg(CCxxx0_FREQ1,    pRfSettings->FREQ1);
    SpiWriteReg(CCxxx0_FREQ0,    pRfSettings->FREQ0);
    SpiWriteReg(CCxxx0_MDMCFG4,  pRfSettings->MDMCFG4);
    SpiWriteReg(CCxxx0_MDMCFG3,  pRfSettings->MDMCFG3);
    SpiWriteReg(CCxxx0_MDMCFG2,  pRfSettings->MDMCFG2);
    SpiWriteReg(CCxxx0_MDMCFG1,  pRfSettings->MDMCFG1);
    SpiWriteReg(CCxxx0_MDMCFG0,  pRfSettings->MDMCFG0);
    SpiWriteReg(CCxxx0_CHANNR,   pRfSettings->CHANNR);
    SpiWriteReg(CCxxx0_DEVIATN,  pRfSettings->DEVIATN);
    SpiWriteReg(CCxxx0_FREND1,   pRfSettings->FREND1);
    SpiWriteReg(CCxxx0_FREND0,   pRfSettings->FREND0);
    SpiWriteReg(CCxxx0_MCSM0 ,   pRfSettings->MCSM0 );
    SpiWriteReg(CCxxx0_FOCCFG,   pRfSettings->FOCCFG);
    SpiWriteReg(CCxxx0_BSCFG,    pRfSettings->BSCFG);
    SpiWriteReg(CCxxx0_AGCCTRL2, pRfSettings->AGCCTRL2);
 	SpiWriteReg(CCxxx0_AGCCTRL1, pRfSettings->AGCCTRL1);
    SpiWriteReg(CCxxx0_AGCCTRL0, pRfSettings->AGCCTRL0);
    SpiWriteReg(CCxxx0_FSCAL3,   pRfSettings->FSCAL3);
    SpiWriteReg(CCxxx0_FSCAL2,   pRfSettings->FSCAL2);
 	SpiWriteReg(CCxxx0_FSCAL1,   pRfSettings->FSCAL1);
    SpiWriteReg(CCxxx0_FSCAL0,   pRfSettings->FSCAL0);
    SpiWriteReg(CCxxx0_FSTEST,   pRfSettings->FSTEST);
    SpiWriteReg(CCxxx0_TEST2,    pRfSettings->TEST2);
    SpiWriteReg(CCxxx0_TEST1,    pRfSettings->TEST1);
    SpiWriteReg(CCxxx0_TEST0,    pRfSettings->TEST0);
    SpiWriteReg(CCxxx0_IOCFG2,   pRfSettings->IOCFG2);
    SpiWriteReg(CCxxx0_IOCFG0,   pRfSettings->IOCFG0);    
    SpiWriteReg(CCxxx0_PKTCTRL1, pRfSettings->PKTCTRL1);
    SpiWriteReg(CCxxx0_PKTCTRL0, pRfSettings->PKTCTRL0);
    SpiWriteReg(CCxxx0_ADDR,     pRfSettings->ADDR_DEV);
    SpiWriteReg(CCxxx0_PKTLEN,   pRfSettings->PKTLEN);
}
//*********************************************odczyt grupowy zawartoùci rejestr°w 
void SpiReadBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile)  
//we:	adres -adr.pierwszego odczytywanego rejestru, 
//	  *pbufor -wskaËnik do buforu odczytywanych danych
//		ile -iloùä rejestr°w do odczytu		
{
unsigned char x, y, maska, wartosc;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres |READ_BURST;//ustawienie (b.7) -odczyt i (b.6) grupa rejestr°w
	maska =0x80;
	//wys-anie SPI adresu rejestru
	for (x=0; x<8; x++)
	{ 
		if ((adres &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;	
	}          
  	for (y=0; y<ile; y++)
  	{ 
  		wartosc =0;
  		maska =0x80;
  		for (x=0; x<8; x++)
  		{ 
    		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
      		Delay_us(1);
      		if ((IO0PIN & SO_RF_PIN) !=0) wartosc =wartosc |maska;
      		Delay_us(1);
  			IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
  			maska =maska >>1;	  		
   		}  
  		*pbufor =wartosc;
  		pbufor++;
  	}
	IO0SET =IO0SET | CS_RF_PORT;   //CS =1   
}
//***************************************odczyt zawartoùci pojedynczego rejestru 
unsigned char SpiReadReg(unsigned char adres)  
//we: adres -adres rejestru
{
unsigned char x, maska, wartosc;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres & 0x3f;//wyzerowany b.7 -zapis i b.6 -grupa rejestr°w
	adres =adres |READ_SINGLE;//ustawienie b.7 -odczyt
	maska =0x80;
	//wys-anie SPI adresu rejestru
	for (x=0; x<8; x++)
	{ 
		if ((adres &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;	
	}          
  wartosc =0;
  maska =0x80;
  for (x=0; x<8; x++)   //odczyt zawartoùci rejestru
  {
  	IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
    Delay_us(1);
    if ((IO0PIN & SO_RF_PIN) !=0) wartosc =wartosc |maska;
    Delay_us(1);
	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	maska =maska >>1;	  		
  } 
  IO0SET =IO0SET | CS_RF_PORT;   //CS =1   
  return wartosc;
}
//***********************************grupowy zapis do kolejnych rejestr°w  
void SpiWriteBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile)    
{  
unsigned char x, y, maska, wartosc;
																			 //we:	adres -adr.pierwszego zapisywanego rejestru, 
	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0								 //	  *pbufor -wskaËnik do tablicy z wartoùciami wpisywanymi do rejestr°w
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0									 //		ile -iloùä rejestr°w zo zapisu	
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres & 0x3f;//wyzerowanie (b.7) -zapis
	adres =adres |WRITE_BURST;//ustawienie (b.6) -zapis do grupy rejestr°w
	maska =0x80;
	//wys-anie SPI adresu rejestru
	for (x=0; x<8; x++)
	{ 
		if ((adres &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;	
	}
	
	for (y=0; y<ile; y++)
	{
	    //wys-anie SPI wartoùci wpisywanych do rejestr°w 
	  	maska =0x80;
	  	wartosc =*pbufor;
		for (x=0; x<8; x++)
		{ 
			if ((wartosc &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
			else IO0SET =IO0SET | SI_RF_PORT;//SI=1
			Delay_us(1);
			IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
			Delay_us(1);
			IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
			maska =maska >>1;	
		}
	  	pbufor++;
	}
	IO0SET =IO0SET | CS_RF_PORT;   //CS =1   
}
//**************************wys-anie pojedynczego rozkazu 
void SpiWriteCommand(unsigned char adres)
{ 
unsigned char x, maska;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 
     
	adres =adres & 0x3f;//wyzerowanie (b.7) -zapis, (b.6) -do pojedynczego rejestru 
	maska =0x80;
	//wys-anie SPI adresu rejestru
	for (x=0; x<8; x++)
	{ 
		if ((adres &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;	
	}
    IO0SET =IO0SET | CS_RF_PORT;   //CS =1   
}
//******************************************zapis wartoùci do pojedynczego rejestru
void SpiWriteReg(unsigned char adres, unsigned char wartosc)//we:	adres -adres rejestru, wartosc -wpisywana wartoùä 
{    
unsigned char x, maska;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 
	adres =adres & 0x3f;//wyzerowanie (b.7) -zapis, (b.6) -do pojedynczego rejestru 
	maska =0x80;
	for (x=0; x<8; x++)	 	//wys-anie SPI adresu rejestru
	{ 
		if ((adres &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;	
	}
	maska =0x80;   //wys-anie SPI wartoùci wpisywanej do rejestru 
	for (x=0; x<8; x++)
	{ 
		if ((wartosc &maska) ==0) IO0CLR =IO0CLR | SI_RF_PORT;//SI=0
		else IO0SET =IO0SET | SI_RF_PORT;//SI=1
		Delay_us(1);
		IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
		Delay_us(1);
		IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
		maska =maska >>1;	
	}
	IO0SET =IO0SET | CS_RF_PORT;   //CS =1  
}

//*****************************************************************wyr°wnanie wskaËnik°w dost‡pu do bufora
void WyrownanieWskaznikowBufora(void)
{uart_buf_pointer =0;}


void tc0 (void) __irq  
{
  T0IR        = 1;                            // Clear interrupt flag
  VICVectAddr = 0;							 //adres wektowa 0
  
  request_trans_flag =0;   
  Test(); 
}

//*****************************************timer init
void init_timer (void) 
{
 T0MR0 = 1499999;                             // T0MR0 = 3000000 -15MHz taimer  
 
  T0MCR = 3;                                  // Interrupt and Reset on MR0
  T0TCR = 1;                                  // Timer0 Enable
  VICVectAddr0 = (unsigned long)tc0;          // set interrupt vector in 0
  VICVectCntl0 = 0x20 | 4;                    // use it for Timer 0 Interrupt
  VICIntEnable = 0x00000010;                  // Enable Timer0 Interrupt
}

//***************************************delay wykozystywany w petli
void delay (unsigned int del_time) 
{ 
  unsigned int i;
  
  for(i = 0; i < del_time; i++);	   
}


void init_ad(void)
{ 
	AD0CR = BIT13|BIT14| BIT21;
	PINSEL1 |= BIT24|BIT26;	
}
 //*******************************************adc
int get_ad(int kanal)
{
	AD0CR &=~(0XFF);	//czysc wybor kanalow
	AD0CR |= 1<<kanal;	//wybor kanalu
	AD0CR |=  BIT24;   //start!
	while ((AD0CR & BIT31) == 0);	   //czekaj na koniec konwersji
	return 	 (AD0CR >> 6) & 0x03FF;		//zwracaj wartosc	   
}

/* Get ADC Value */
int get_ad_chanel(int Chan)
{
	AD0CR &= ~AD0CR_SEL_MASK;	
	AD0CR |= 1<<Chan & AD0CR_SEL_MASK;
	AD0CR |= 1<<AD0CR_START_BIT;
	while(!(AD0GDR & AD0GDR_DONE));
	return (AD0GDR >> 6) & 0x3FF;	  
}
//**********************************************PWM
void pwm_init(void)
{
	PWMPR = 9;		//Preskaler CCLK/10 = 6MHz
	PWMMCR |= BIT0;	 //Zgodne zbocze kasuje timer
	//PWMPCR |= PWMPCR_PWMENA5 |PWMPCR_PWMENA6|PWMPCR_PWMENA4; 	   	//Ustaw PWM jako wyjscie
    PWMPCR |= BIT14|BIT12|BIT11|BIT10|BIT9 ; 	   	//Ustaw PWM jako wyjscie

	PWMMR0 = 100;  //Ustaw rejestr zakres 100 

	PWMMR1 = 0;
	PWMMR2 = 0;
	PWMMR3 = 0;
	PWMMR4 = 0;		 	//PWM= 0%	   WARTOSC POCZATKOWA
	PWMMR6 = 0;

	PWMLER = (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<6)|(1<<0);	  	//Wykonaj zapis MR0 i MR5	-TAKI SAM WSP. WYPELEINIA
  	//PINSEL0 = BIT1|BIT3|BIT15|BIT17|BIT19|BIT10 ;		 	//Wlacz alternatywny pin PWM5
	PINSEL0 = BIT1|BIT3|BIT15|BIT17|BIT19;
	PWMTCR = BIT1;	   	//Zeruj licznik i preskaler
	PWMTCR = BIT3 | BIT0;	   	//Wlacz licznik PWM
}  
//***************************************************main
int main(void)
{
static unsigned long delay_loop;

	Inicjacja(); 
	SpiWriteCommand(CCxxx0_SRX); //inicjacja trybu Rx transceiwera
	delay_loop =0x000f0000;
 	init_ad();
	IODIR1 = 0xffFF0000;	//dla ledow
	pwm_init();
	IOSET1=BIT17;
	while (1)
	{	
	  
		 	   	
	/*	if (((IO0PIN & SW_KL) ==0 && delay_loop ==0))	  	//test
	   	{  
	  		 	Test();  
		request_trans_flag =0;	 
		}    */   
	    if ((IO0PIN & GDO_RF_PIN) !=0) //inicjacja odbioru danych torem radiowym
	    {  
	    	ModRxStart();
	    }
		if (delay_loop !=0) delay_loop--;
				
	} 
}

//---------------------------------------------------
void Test(void)	   //do wyslania
{      
	 	int chan1,chan2;
		int dana_x=0,dana_y=0;
		int dana_xx,dana_yy,dana,dana_yyy;
		chan1 = get_ad_chanel(1);		
		chan2 = get_ad_chanel(2);
		dana_x=	(chan1 >> 2); //do warotsci 8bitowej				to bede wysylal
	    dana_y=	(chan2 >> 2); //do warotsci 8bitowej
	    dana_xx = ((dana_x >> 4) & 0x0F);
	    dana_yy = ((dana_y >> 4) & 0x0F);
		//chan1=0; chan2=0;
		switch(dana_yy)		
	 {
	   case 4 :
			 	 dana_yyy=0x1;		
			 break;
	   case 5 :
			 	 dana_yyy=0x2;		
			 break;
	   case 6:
				 dana_yyy=0x3;	
			break;
	   case 7:	 					  //srodek
				 dana_yyy=0x4;			
			break;
	   case 8:							//prawo
				 dana_yyy=0x5;	   		
			break;
	   case 9:
				 dana_yyy=0x6;	
			break;
	   case 10:
				 dana_yyy=0x7;	
			break;
	 }	
	 dana=(dana_xx|(dana_yyy<<4));	
	
	 if (((IO0PIN & SW_KL) ==0 ))	  	//test
	   	{  
	  	  dana&=(0x7|(0x4<<4));
		}  		 
	  	bufor[(unsigned char)(0)] = dana;		
                                        
	//	for(o;o<100;o++)
	//	{for(i;i<1000000;i++);}
	      dana=0x00;	 				
		  RadioTransAndWait(2);
		  	  
}

void odebranie(void)
{
	  int val=0; 
	  int  val_jazda=0,val_ster=0;
	  	 val |= bufor[0];
		 	/*	 
		 	 

	  IOCLR1 = 0x00FFF000;						// wy≥πcz diody
      IOSET1 = 0x00FFF000 & (val << 16);		// 8 bardziej znaczπcych bitÛw  */
	  //sterowanie przod/tyl
	 val_jazda = (val & 0x0f);
	 val_ster =	 ((val>>4)& 0x0f);
	 switch(val_jazda)		
	 {
	   case 2 :
			 PWMMR2=100;	
			 PWMMR4=0; 
			 //jazda_enable;  		
			 break;
	   case 3:
     		 PWMMR2=85;
			 PWMMR4=0; 
			 //jazda_enable; 	   		
			break;
	   case 4:	 					  
			 PWMMR2=70; 
			 PWMMR4=0; 
			 jazda_enable; 	   		
			break;
	   case 5:						
     		PWMMR2=50;
			PWMMR4=0; 
		//	jazda_enable;   		
			break;
	   case 6:
     		PWMMR2=30;
			PWMMR4=0; 
			//jazda_enable;  		
			break;
	   case 7:	 					  //srodek
			 PWMMR2=0; 
			 PWMMR4=0; 
		//	 jazda_disable; 	   		
			break;
	   case 8:							//prawo
     		PWMMR2=0;
			PWMMR4=30; 
		//	jazda_enable;   		
			break;
	   case 9:
     		PWMMR2=0;
			PWMMR4=50; 
		//	jazda_enable;  		
			break;
	   case 10:
     		 PWMMR2=0;
			 PWMMR4=70; 
		//	 jazda_enable; 	   		
			break;
	   case 11:	 					
			 PWMMR2=0; 
			 PWMMR4=85; 
		//	 jazda_enable; 	   		
			break;
	   case 12:							
     		PWMMR2=0;
			PWMMR4=100; 
		//	jazda_enable;   		
			break;
	  }	
  			  
//sterowanie na boki 	 
	 switch(val_ster)	
	 {
	  case 0 :
			 PWMMR1=100;	
			 PWMMR3=0;
			 ster_enable;   		
			 break;
	   case 1 :
			 PWMMR1=100;	
			 PWMMR3=0;
			 ster_enable;   		
			 break;
	   case 2:
     		 PWMMR1=100;
			 PWMMR3=0;
			 ster_enable;   	   		
			break;
	   case 3:	 				
			 PWMMR1=100; 
			 PWMMR3=0;
			 ster_enable;   	   		
			break;
	   case 4:					  //srodek			
     		PWMMR1=0;
			PWMMR3=0;
			ster_disable;     		
			break;
	   case 5:
     		PWMMR1=0;
			PWMMR3 =30; 
			ster_enable;   		
			break;
	   case 6:							
     		PWMMR1=0;
			PWMMR3=100;
			ster_enable;     		
			break;
	   case 7:
     		PWMMR1=0;
			PWMMR3 =100; 
			ster_enable;   		
			break;
	   case 8:
     		PWMMR1=0;
			PWMMR3 =100; 
			ster_enable;   		
			break;
	   case 9:
     		PWMMR1=0;
			PWMMR3 =100; 
			ster_enable;   		
			break;
			}					
			
	 val_jazda=0;
	 val_ster=0;	  
	  PWMLER = (1<<1)|(1<<2)|(1<<3)|(1<<4); 
	  
	  
	  
	  val=0;


}


