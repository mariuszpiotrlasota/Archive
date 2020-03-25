/*****************************************************************
 * definicje sta³ych i zmiennych radio modemu                    *
 * dla modu³u mif7 z transceiwerem CC1100EM                      *   
 *****************************************************************/   


#define D5_LED	BIT22
#define D4_LED	BIT23


#define BUFOR_SIZE			64//rozmiar bufora

#define CS_RF_PORT			BIT23 //P0.23 wyjœcie CS
#define SI_RF_PORT			BIT22 //P0.22 wyjœcie SI
#define SCLK_RF_PORT		BIT21 //P0.21 wyjœcie SCLK 
#define SO_RF_PIN			BIT20 //P0.20 wejœcie SO
#define GDO_RF_PIN			BIT19 //P0.19 wejœcie GDO

#define SW_KL 	 			BIT13//P0.27 przycisk sw
#define LED_TRANS_PORT  	BIT17 //P0.17 wyprowadzenie Out led Tx   
#define LED_TRANS_PIN		BIT17
#define LED_REC_PORT		BIT16 //P0.16 wyprowadzenie Out led Rx 
#define LED_ERROR_PORT		BIT10 //P0.18 wyprowadzenie Out led Error
#define DSR_SIGNAL			BIT11 //P0.21 wyprowadzenie DSR  

#define TEST_ILE_DANYCH	23//iloœæ danych w transmisji testowej  
unsigned char const test_txt_eep[] ={""}; 
char const takst[] ={""};

//definicje, sta³e i zmienne zwi¹zane z UART-em
#define TRANSMISJA_1_RAMKI_9600		1.15//czas transmisji 1 ramki UART-em w milisekundach

unsigned char test_tx_rx;	//flaga transmisji testowej  
unsigned char rx_start_rec_flag;//flaga gotowoœci do odbioru danych z modu³u radiowego   
unsigned char uart_write_flag;//flaga trwania transmisji UARTEM  
unsigned char uart_rxd_flag;//flaga sygnalizuj¹ca umieszczenie w buforze uart_buf_in bajtu odebranego UARTEM  
unsigned char request_trans_flag;//1 :transmisja testowa jest odpowiedzi¹ na transmisjê testow¹ partnera

unsigned char uart_bufor_ile_odebranych;//licznik bajtów odebranych UARTEM   
unsigned char uart_buf_pointer;//licznik aktualnej pozycji w buforze odbioru z UARTU
unsigned char uart_txd_count;//licznik bajtów transmitowanych UARTEM              
unsigned char uart_buf_in;//tymczasowy bufor danej odebranej UARTEM
unsigned char bufor[BUFOR_SIZE+2];//bufor transmisji  


// CC2500/CC1100 STROBE, CONTROL AND STATUS REGSITER
#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CCxxx0_SYNC1        0x04        // Sync word, high byte
#define CCxxx0_SYNC0        0x05        // Sync word, low byte
#define CCxxx0_PKTLEN       0x06        // Packet length
#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
#define CCxxx0_ADDR         0x09        // Device address
#define CCxxx0_CHANNR       0x0A        // Channel number
#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
#define CCxxx0_FREQ2        0x0D        // Frequency control word, high byte
#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle byte
#define CCxxx0_FREQ0        0x0F        // Frequency control word, low byte
#define CCxxx0_MDMCFG4      0x10        // Modem configuration
#define CCxxx0_MDMCFG3      0x11        // Modem configuration
#define CCxxx0_MDMCFG2      0x12        // Modem configuration
#define CCxxx0_MDMCFG1      0x13        // Modem configuration
#define CCxxx0_MDMCFG0      0x14        // Modem configuration
#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
#define CCxxx0_MCSM2        0x16        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
#define CCxxx0_AGCCTRL2     0x1B        // AGC control
#define CCxxx0_AGCCTRL1     0x1C        // AGC control
#define CCxxx0_AGCCTRL0     0x1D        // AGC control
#define CCxxx0_WOREVT1      0x1E        // High byte Event 0 timeout
#define CCxxx0_WOREVT0      0x1F        // Low byte Event 0 timeout
#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
#define CCxxx0_FREND1       0x21        // Front end RX configuration
#define CCxxx0_FREND0       0x22        // Front end TX configuration
#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
#define CCxxx0_FSTEST       0x29        // Frequency synthesizer calibration control
#define CCxxx0_PTEST        0x2A        // Production test
#define CCxxx0_AGCTEST      0x2B        // AGC test
#define CCxxx0_TEST2        0x2C        // Various test settings
#define CCxxx0_TEST1        0x2D        // Various test settings
#define CCxxx0_TEST0        0x2E        // Various test settings

// Strobe commands
#define CCxxx0_SRES         0x30        // Reset chip.
#define CCxxx0_SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        // If in RX/TX: Go to a wait state where only the synthesizer is
                                        // running (for quick RX / TX turnaround).
#define CCxxx0_SXOFF        0x32        // Turn off crystal oscillator.
#define CCxxx0_SCAL         0x33        // Calibrate frequency synthesizer and turn it off
                                        // (enables quick start).
#define CCxxx0_SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                        // MCSM0.FS_AUTOCAL=1.
#define CCxxx0_STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        // Only go to TX if channel is clear.
#define CCxxx0_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                        // Wake-On-Radio mode if applicable.
#define CCxxx0_SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
#define CCxxx0_SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CCxxx0_SPWD         0x39        // Enter power down mode when CSn goes high.
#define CCxxx0_SFRX         0x3A        // Flush the RX FIFO buffer.
#define CCxxx0_SFTX         0x3B        // Flush the TX FIFO buffer.
#define CCxxx0_SWORRST      0x3C        // Reset real time clock.
#define CCxxx0_SNOP         0x3D        // No operation. May be used to pad strobe commands to two
                                        // bytes for simpler software.

#define CCxxx0_PARTNUM      0x30
#define CCxxx0_VERSION      0x31
#define CCxxx0_FREQEST      0x32
#define CCxxx0_LQI          0x33
#define CCxxx0_RSSI         0x34
#define CCxxx0_MARCSTATE    0x35
#define CCxxx0_WORTIME1     0x36
#define CCxxx0_WORTIME0     0x37
#define CCxxx0_PKTSTATUS    0x38
#define CCxxx0_VCO_VC_DAC   0x39
#define CCxxx0_TXBYTES      0x3A
#define CCxxx0_RXBYTES      0x3B

#define CCxxx0_PATABLE      0x3E
#define CCxxx0_TXFIFO       0x3F
#define CCxxx0_RXFIFO       0x3F


typedef struct S_RF_SETTINGS{
    unsigned char FSCTRL1;   // Frequency synthesizer control.
    unsigned char FSCTRL0;   // Frequency synthesizer control.
    unsigned char FREQ2;     // Frequency control word, high byte.
    unsigned char FREQ1;     // Frequency control word, middle byte.
    unsigned char FREQ0;     // Frequency control word, low byte.
    unsigned char MDMCFG4;   // Modem configuration.
    unsigned char MDMCFG3;   // Modem configuration.
    unsigned char MDMCFG2;   // Modem configuration.
    unsigned char MDMCFG1;   // Modem configuration.
    unsigned char MDMCFG0;   // Modem configuration.
    unsigned char CHANNR;    // Channel number.
    unsigned char DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    unsigned char FREND1;    // Front end RX configuration.
    unsigned char FREND0;    // Front end RX configuration.
    unsigned char MCSM0;     // Main Radio Control State Machine configuration.
    unsigned char FOCCFG;    // Frequency Offset Compensation Configuration.
    unsigned char BSCFG;     // Bit synchronization Configuration.
    unsigned char AGCCTRL2;  // AGC control.
	unsigned char AGCCTRL1;  // AGC control.
    unsigned char AGCCTRL0;  // AGC control.
    unsigned char FSCAL3;    // Frequency synthesizer calibration.
    unsigned char FSCAL2;    // Frequency synthesizer calibration.
 	unsigned char FSCAL1;    // Frequency synthesizer calibration.
    unsigned char FSCAL0;    // Frequency synthesizer calibration.
    unsigned char FSTEST;    // Frequency synthesizer calibration control
    unsigned char TEST2;     // Various test settings.
    unsigned char TEST1;     // Various test settings.
    unsigned char TEST0;     // Various test settings.
    unsigned char IOCFG2;    // GDO2 output pin configuration
    unsigned char IOCFG0;    // GDO0 output pin configuration
    unsigned char PKTCTRL1;  // Packet automation control.
    unsigned char PKTCTRL0;  // Packet automation control.
    unsigned char ADDR_DEV;  // Device address.
    unsigned char PKTLEN;    // Packet length.	
} RF_SETTINGS;
 
/* Chipcon */
/* Product = CC1100 */
/* Chip version = E */
/* Crystal accuracy = 40 ppm */
/* X-tal frequency = 26 MHz */
/* RF output power = 10 dBm */
/* RX filterbandwidth = 58.035714 kHz */
/* Deviation = 5 kHz */
/* Datarate = 2.398968 kbps */
/* Modulation = (0) 2-FSK */
/* Manchester enable = (0) Manchester disabled */
/* RF Frequency = 432.999817 MHz */
/* Channel spacing = 199.951172 kHz */
/* Channel number = 0 */
/* Optimization = - */
/* Sync mode = (3) 30/32 sync word bits detected */
/* Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX */
/* CRC operation = (1) CRC calculation in TX and CRC check in RX enabled */
/* Forward Error Correction = (0) FEC disabled */
/* Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word. */
/* Packetlength = 255 */
/* Preamble count = (2)  4 bytes */
/* Append status = 1 */
/* Address check = (0) No address check */
/* FIFO autoflush = 0 */
/* Device address = 0 */
/* GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet */
/* GDO2 signal selection = (11) Serial Clock */ 

RF_SETTINGS rfSettings = {
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0xF6,   // MDMCFG4   Modem configuration.
    0x83,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x15,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x03,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
    0xA9,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x0B,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xFF    // PKTLEN    Packet length.
}; 

// PATABLE (0 dBm output power)
unsigned char paTable[] = {0x51};

// Definitions to support burst/single access:
#define WRITE_BURST     0x40
#define READ_SINGLE     0x80
#define READ_BURST      0xC0
#define BYTES_IN_RXFIFO 0x7f
#define LQI				1
#define CRC_OK			0x80




