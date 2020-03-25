 /*****************************************************************
  * Przyk³adowy program radio-modemu opartego o p³ytkê MIF7       *
  * z LPC210x i transceiwerem CC1100EM                            *
  * www.ars.info.pl ARIES										  *
  *****************************************************************/
                  
#include <stdio.h>                         /* standard I/O .h-file */
#include <LPC21xx.H>                       /* LPC21xx definitions  */
#include <string.h>
#include <arm1_11_mif7.h>

//procedury inicjacji
void Inicjacja(void);
//procedura pauzy w ms
void Delay_ms(int czas);
//procedura pauzy w us
void Delay_us(int czas);
//odczyt danych z modu³u radiowego 
unsigned char ModRxStart(void);
//transmisja radiowa i oczekiwanie na przes³anie UARTEM przez partnera otrzymanych danych  
void RadioTransAndWait(unsigned char tx_ile);
//sekwencja zerowania transceiwera po w³¹czeniu zasilania
void Rf_Power_Up_Reset(void);
//transmisja pakietu danych  
void Rf_Send_Packet(unsigned char *txBuffer, unsigned char size);
//programowanie rejestrów CC1100
void RfWriteRfSettings(RF_SETTINGS *pRfSettings);
//odczyt grupowy zawartoœci rejestrów 
void SpiReadBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile);
//odczyt zawartoœci pojedynczego rejestru 
unsigned char SpiReadReg(unsigned char adres); 
//grupowy zapis do kolejnych rejestrów  
void SpiWriteBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile);
//wys³anie pojedynczego rozkazu 
void SpiWriteCommand(unsigned char adres);
//zapis wartoœci do pojedynczego rejestru
void SpiWriteReg(unsigned char adres, unsigned char wartosc); 
//test toru polegaj¹cy na wys³aniu sekwencji test_txt 
void Test(void);
//inicjacja UART0
void UART0_Init(void);
//odczyt danych z UARTU    
void UartRxStart(void);
//wyrównanie wskaŸników dostêpu do bufora
void WyrownanieWskaznikowBufora(void);


void main(void)
{
static unsigned long delay_loop;

	Inicjacja();
	//inicjacja trybu Rx transceiwera
 	SpiWriteCommand(CCxxx0_SRX); 
	delay_loop =0x000f0000;

	while (1)
	{
 		//inicjacja transmisji testowej jeœli naciœniêty przycisk i up³ynê³o 0.5s od poprzedniej
	   	if ((IO0PIN & SW_KL) ==0 && delay_loop ==0)
	   	{               
	   	  	request_trans_flag =0; 
	   	  	Test();  
		    delay_loop =0x000f0000; 
	   	}       
	    // Wait for GDO0 to be set -> sync received   
	    //inicjacja odbioru danych torem radiowym
	    if ((IO0PIN & GDO_RF_PIN) !=0) 
	    {  
	    	ModRxStart();
	    }
		if (delay_loop !=0) delay_loop--;
		//inicjacja odbioru danych UART-em
		if ((U0LSR & 0x01) !=0)
		{
			uart_rxd_flag =1;
			UartRxStart(); 
		}
	}
}

//--------------------
//procedura pauzy w ms
void Delay_ms(int czas)
//we: czas pauzy 0x0001 -0xffff
{
#define PETLA_1MS	10006

	czas =czas *PETLA_1MS;
	while (czas !=0) czas--;
}
//--------------------
//procedura pauzy w us
void Delay_us(int czas)
//we: czas pauzy 0x0001 -0xffff
{
#define PETLA_1US	10

	czas =czas *PETLA_1US;
	while (czas !=0) czas--;
}
//-------------------
//procedury inicjacji
void Inicjacja(void)
{
	VPBDIV = 0x02;	//Divide Pclk by two 30MHz

	//zerowanie kluczowych flag  
	rx_start_rec_flag =0;
	uart_rxd_flag =0;      

	IO0DIR = IO0DIR | CS_RF_PORT |SI_RF_PORT | SCLK_RF_PORT | //pin P0.2, P0.3, P0.4, 
	          LED_TRANS_PORT | LED_REC_PORT | LED_ERROR_PORT |//P0.16, P0.17, P0.18, P0.24 
			  DSR_SIGNAL; 									  //configured as output
	//porty ustawione w stanie wysokim
	IO0SET = IO0SET | CS_RF_PORT;
	//porty ustawione w stanie niskim
	IO0CLR =IO0CLR | DSR_SIGNAL;
	//RESET start 
 	IO0CLR = IO0CLR | LED_TRANS_PORT;//LED on
    IO0CLR = IO0CLR | LED_REC_PORT;  	 
	Delay_ms(100); //pauza 100ms
	Rf_Power_Up_Reset(); 
	RfWriteRfSettings(&rfSettings);
	SpiWriteBurstReg(CCxxx0_PATABLE, &paTable[0], sizeof(paTable));
 	IO0SET = IO0CLR | LED_TRANS_PORT;//LED on
    IO0SET = IO0CLR | LED_REC_PORT; 
	 	 
	UART0_Init();//inicjacja UART0
}
//--------------------------------
//odczyt danych z modu³u radiowego 
unsigned char ModRxStart(void)
//wy:	iloœæ odebranych danych, 0- prawdopodobnie nast¹pi³ b³¹d w czasie odbioru pakietu
{  
unsigned char status[2];
static unsigned char packed_length; 
unsigned char x;
               
    IO0SET = IO0SET | DSR_SIGNAL;//wstrzymanie odbioru nowych danych portem RS   
  
  	IO0CLR = IO0CLR | LED_REC_PORT;  
	WyrownanieWskaznikowBufora();//wyrównanie wskaŸników dostêpu do bufora    
		
	// Wait for GDO0 to be cleared -> end of packet 
	// oczekiwanie na stan niski wyjœcia DGO0 sygnalizuj¹cy odbiór kompletnego
	// pakietu danych
 	while ((IO0PIN & GDO_RF_PIN) !=0); 
  	//odczyt rejestru statusu
  	SpiReadBurstReg(CCxxx0_RXBYTES, &packed_length, 1); 
  	packed_length =packed_length & BYTES_IN_RXFIFO;
  	if (packed_length !=0)//odczytano co najmniej 1 bajt
  	{          
   		//odczyt 1 przes³anego bajtu zawieraj¹cego d³ugoœæ przes³anego pakietu danych
   		packed_length =SpiReadReg(CCxxx0_RXFIFO);
   		if (packed_length <=BUFOR_SIZE)
   		{
   			SpiReadBurstReg(CCxxx0_RXFIFO, &bufor[0], packed_length);    
   			//odczyt 2 bajtów statusu: status[0]=RSSI, status[1] =LQI
   			SpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 
   			if ((status[LQI] &CRC_OK) ==0) packed_length =0;//b³¹d sumy CRC przes³anego pakietu
   		} 
   		else
   		{  
   			//b³¹d, przes³any pakiet > od rozmiaru buforu
   	  		// Flush RX FIFO
      		SpiWriteCommand(CCxxx0_SFRX);
      		packed_length =0; 	
   		}
  	}  	
	SpiWriteCommand(CCxxx0_SFRX);  
  	SpiWriteCommand(CCxxx0_SIDLE); 
  	if (packed_length !=0)
  	{	
	    uart_buf_pointer =0;
		uart_txd_count =packed_length;
   		//inicjacja transmisji UARTEM danych odebranych z transceiwera
    	while (!(U0LSR & 0x20));//oczekiwanie na zwolnienie rejestru transmisji
		while (uart_txd_count !=0)
		{
	    	U0THR = bufor[uart_buf_pointer];//kolejna dana do rejestru transmisji
			uart_buf_pointer++;
			uart_txd_count--;
			while (!(U0LSR & 0x20));//oczekiwanie na zwolnienie rejestru transmisji
    	}
  	} 
	Delay_ms(1); 
  	SpiWriteCommand(CCxxx0_SRX);    
  	IO0SET = IO0SET | LED_REC_PORT;  

	while(1)
	{	
		//sprawdzenie czy odebrana transmisja jest transmisj¹ testow¹
		if (packed_length !=sizeof(test_txt_eep)-1) break;
		for (x=0; x<sizeof(test_txt_eep)-1; x++)
		{
			if (x ==0) continue;//nie kontrolowane znaki w transmisji
			if (bufor[x] !=test_txt_eep[x]) break;
		}            
		if (x !=(sizeof(test_txt_eep)-1)) break;
 
 	  //przes³anie w trybie testowym    
    	if (bufor[0] !='T')
    	{    
    		//odebrana transmisja testowa by³a odpowiedzi¹, 
    		//wysy³anie odpowiedzi na odpowiedŸ jest zabronione
      	Delay_ms(200); //pauza 200ms
      	break;
    	}
 
    	//wysy³anie odpowiedzi na transmisjê testow¹ partnera   
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
//---------------------------------------------------------------------------------------
//transmisja radiowa i oczekiwanie na przes³anie UARTEM przez partnera odebranych danych  
void RadioTransAndWait(unsigned char tx_ile)
//we:	tx_ile -ile bajtów do wys³ania  
//		nr_uart -numer szybkoœci UARTA
{  
float czas_transmisji_uartem;
unsigned int pauza;
     

	memmove(&bufor[1], &bufor[0], tx_ile);    
	//wpisanie na 1 pozycjê buforu iloœci transmitowanych danych w pakiecie
	bufor[0] =tx_ile;
	//transmisja danych torem radiowym + 1 bajt d³ugoœci transmisji
	Rf_Send_Packet(&bufor[0], tx_ile+1);
  	//maksymalny czas oczekiwania na wys³anie przez partnera UARTEM
  	//odebrnych danych =(iloœæ danych+1 * czas transmisji UARTEM 1 ramki) 
  	czas_transmisji_uartem =((float)(tx_ile +5) *TRANSMISJA_1_RAMKI_9600);
  	pauza =(unsigned int)(czas_transmisji_uartem);
  	Delay_ms(pauza);
}

//-------------------------------------------------------
//sekwencja zerowania transceiwera po w³¹czeniu zasilania
void Rf_Power_Up_Reset(void)
{ 
unsigned char maska, x;

	IO0SET = IO0SET | SCLK_RF_PORT;//SCLK=1
	IO0CLR =IO0CLR | SI_RF_PORT;   //SI =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	Delay_us(2);
	IO0SET =IO0SET | CS_RF_PORT;   //CS =1 
	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	Delay_us(40); //pauza 40us
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 
     
	maska =0x80;
	//wys³anie przez SPI komendy CCxxx0_SRES
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
//-------------------------
//transmisja pakietu danych  
void Rf_Send_Packet(unsigned char *txBuffer, unsigned char size)
{               
	SpiWriteCommand(CCxxx0_SIDLE); 
	SpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size); 
	SpiWriteBurstReg(CCxxx0_PATABLE, &paTable[0], sizeof(paTable));
	SpiWriteCommand(CCxxx0_STX);	 
	//oczekiwanie na stan wysoki na wyjœciu GDO -> sync transmitted
	while ((IO0PIN & GDO_RF_PIN) ==0);  
	IO0CLR = IO0CLR | LED_TRANS_PORT;//LED on
	//oczekiwanie na stan niski wyjœcia GDO -> end of packet
	while ((IO0PIN & GDO_RF_PIN) !=0);
	IO0SET = IO0SET | LED_TRANS_PORT;//LED off
	SpiWriteCommand(CCxxx0_SIDLE); 
	Delay_ms(1); 
    SpiWriteCommand(CCxxx0_SRX);
	Delay_ms(1);
}
//------------------------------
//programowanie rejestrów CC1100
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
//-----------------------------------
//odczyt grupowy zawartoœci rejestrów 
void SpiReadBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile)  
//we:	adres -adr.pierwszego odczytywanego rejestru, 
//	  *pbufor -wskaŸnik do buforu odczytywanych danych
//		ile -iloœæ rejestrów do odczytu		
{
unsigned char x, y, maska, wartosc;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres |READ_BURST;//ustawienie (b.7) -odczyt i (b.6) grupa rejestrów
	maska =0x80;
	//wys³anie SPI adresu rejestru
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
//---------------------------------------
//odczyt zawartoœci pojedynczego rejestru 
unsigned char SpiReadReg(unsigned char adres)  
//we: adres -adres rejestru
{
unsigned char x, maska, wartosc;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres & 0x3f;//wyzerowany b.7 -zapis i b.6 -grupa rejestrów
	adres =adres |READ_SINGLE;//ustawienie b.7 -odczyt
	maska =0x80;
	//wys³anie SPI adresu rejestru
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
  //odczyt zawartoœci rejestru
  for (x=0; x<8; x++)
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
//------------------------------------
//grupowy zapis do kolejnych rejestrów  
void SpiWriteBurstReg(unsigned char adres, unsigned char *pbufor, unsigned char ile)    
//we:	adres -adr.pierwszego zapisywanego rejestru, 
//	  *pbufor -wskaŸnik do tablicy z wartoœciami wpisywanymi do rejestrów
//		ile -iloœæ rejestrów zo zapisu		
{  
unsigned char x, y, maska, wartosc;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres & 0x3f;//wyzerowanie (b.7) -zapis
	adres =adres |WRITE_BURST;//ustawienie (b.6) -zapis do grupy rejestrów
	maska =0x80;
	//wys³anie SPI adresu rejestru
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
	    //wys³anie SPI wartoœci wpisywanych do rejestrów 
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
//-----------------------------
//wys³anie pojedynczego rozkazu 
void SpiWriteCommand(unsigned char adres)
{ 
unsigned char x, maska;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 
     
	adres =adres & 0x3f;//wyzerowanie (b.7) -zapis, (b.6) -do pojedynczego rejestru 
	maska =0x80;
	//wys³anie SPI adresu rejestru
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
//---------------------------------------
//zapis wartoœci do pojedynczego rejestru
void SpiWriteReg(unsigned char adres, unsigned char wartosc) 
//we:	adres -adres rejestru, wartosc -wpisywana wartoœæ
{    
unsigned char x, maska;

	IO0CLR =IO0CLR | SCLK_RF_PORT; //SCLK =0
	IO0CLR =IO0CLR | CS_RF_PORT;   //CS =0
	while ((IO0PIN & SO_RF_PIN) !=0);//oczekiwanie na stan niski linii SO 

	adres =adres & 0x3f;//wyzerowanie (b.7) -zapis, (b.6) -do pojedynczego rejestru 
	maska =0x80;
	//wys³anie SPI adresu rejestru
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
	//wys³anie SPI wartoœci wpisywanej do rejestru 
	maska =0x80;
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
//---------------------------------------------------
//test toru polegaj¹cy na wys³aniu sekwencji test_txt 
void Test(void)
{      
unsigned char x;
  
  for (x=0; x<TEST_ILE_DANYCH; x++)
  {
    bufor[(unsigned char)(x)] =test_txt_eep[x];
  }   
  //jeœli to odpowiedŸ na transmisjê testow¹ wpisanie do transmitowanego komunikatu kodu 'R'                                                           
  if (request_trans_flag ==1) 
  {
    bufor[0] ='R'; 
  }    
  RadioTransAndWait(TEST_ILE_DANYCH);
}
//---------------
//inicjacja UART0
void UART0_Init(void)
{

  PINSEL0 	= 0x00000005;         /* Enable RxD0 and TxD0              */ 
  U0FCR 	= 7; 				  // Enable and clear FIFO's
  U0LCR 	= 0x00000083;         /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL 	= 0x000000C2;         /* 9600 Baud Rate @ 30MHz VPB Clock  */
  U0DLM 	= 0;
  U0LCR 	= 0x00000003;        /* DLAB = 0                          */
}
//odczyt danych z UARTU    
//---------------------
void UartRxStart(void)
{     
unsigned char mnoznik_pauzy; 
unsigned int czas_1_ramki_uart_us; 
								 
  uart_bufor_ile_odebranych =0;//kasowanie licznika znaków odebranych UARTEM    
  WyrownanieWskaznikowBufora();
  mnoznik_pauzy =3;
  czas_1_ramki_uart_us =(unsigned int)(TRANSMISJA_1_RAMKI_9600 *1000);

  while(1)
  {  
  	if ((U0LSR & 0x01) !=0)
	{
	  uart_buf_in =U0RBR;
	  uart_rxd_flag =1;
	}

    if (uart_rxd_flag ==1)
    {      
      uart_rxd_flag =0;   
      bufor[uart_bufor_ile_odebranych] =uart_buf_in;
      uart_bufor_ile_odebranych++; 
      if (uart_bufor_ile_odebranych >=(BUFOR_SIZE-10))
      {
        //w buforze brak wolnego miejsca na  nowe dane co powoduje 
		//inicjacjê transmisji odebranych danych torem radiowym
		IO0SET =IO0SET | DSR_SIGNAL;//wstrzymanie transmisji nowych danych portem UART-a 
		do
		{
			Delay_us(czas_1_ramki_uart_us *2);
			if ((U0LSR & 0x01) !=0)
			{
				uart_buf_in =U0RBR;
				bufor[uart_bufor_ile_odebranych] =uart_buf_in;
				uart_bufor_ile_odebranych++; 
			}	
		}	  
		while ((U0LSR & 0x01) !=0);
        break;	 
      }
	   
      mnoznik_pauzy =3;	    
    }
    Delay_us(czas_1_ramki_uart_us);
	mnoznik_pauzy--;
	//je¿eli czas przerwy pomiêdzy transmisj¹ kolejnych danych portem UART >2 ramek
	//zakoñczenie odbioru UART-em i przejœcie do wysy³ania odebranych danych
	//torem radiowym
	if (mnoznik_pauzy ==0) 
	{
		Delay_us(czas_1_ramki_uart_us *2);
		if ((U0LSR & 0x01) !=0)
		{
			uart_buf_in =U0RBR;
			bufor[uart_bufor_ile_odebranych] =uart_buf_in;
			uart_bufor_ile_odebranych++; 
		}		  
        break;	 
	}
  } 
  IO0SET =IO0SET | DSR_SIGNAL;//wstrzymanie transmisji nowych danych portem UART-a   
  
  //wysy³anie danych odebranych UARTEM torem radiowym
  test_tx_rx =0; //to nie jest transmisja testowa 
  RadioTransAndWait(uart_bufor_ile_odebranych); 
  IO0CLR =IO0CLR | DSR_SIGNAL;;//odblokowanie transmisji nowych danych portem UART-a       
}
//---------------------------------------
//wyrównanie wskaŸników dostêpu do bufora
void WyrownanieWskaznikowBufora(void)
{

	uart_buf_pointer =0;
}
