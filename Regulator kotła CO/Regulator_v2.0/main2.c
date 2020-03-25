/******************************************************************
#define D8    BIT7//P1,7
#define D7    BIT6//P1,6
#define D6    BIT5//P1,5
#define D5    BIT4//P1.4
#define E     BIT2//P1,2
#define RS    BIT1//P1,1
                                                      *****
 ******************************************************************/

#include  <msp430x13x.h>


#define   E_HIGH          P1OUT |= BIT2
#define   E_LOW           P1OUT &= ~BIT2
#define   RS_HIGH         P1OUT |= BIT1
#define   RS_LOW          P1OUT &= ~BIT1
#define   LCD_Data        P1OUT
#define   LCD_LIGHT_ON    P1OUT |= BIT0
#define   LCD_LIGHT_OFF   P1OUT &= ~BIT0

#define   INPUT           0
#define   OUTPUT          0xff
#define   ON              1
#define   OFF             0
#define   BUF_SIZE        25

#define   _100us     7                 //7 cycles *12 + 20 = 104 / 104*1us = 104us

//LCD commands
#define		DISP_ON			0x0c	        //LCD control constants
#define		DISP_OFF		0x08	        //
#define		CLR_DISP		0x01    	//
#define		CUR_HOME		0x02	        //
#define		ENTRY_INC		0x06            //
#define		DD_RAM_ADDR		0x80	        //
#define		DD_RAM_ADDR2		0xc0	        //
#define		DD_RAM_ADDR3		0x28	        //
#define		CG_RAM_ADDR		0x40	        //

//NAND FLASH 
#define MAX_BLOCK_NUMB		1024

#define TRANS_LDY 		50
#define WRITE_DLY		400
#define ERASE_DLY		4000


#define READ_SPARE			0x50
#define READ_0				0x00
#define READ_1				0x01
#define READ_STATUS			0x70

#define WRITE_PAGE			0x80
#define WRITE_AKN			0x10

#define ERASE_BLOCK			0x60
#define	ERASE_AKN			0xD0

#define DEV_ID				0x90

#define SAMSUNG_ID			0xECE6


unsigned char i, proba, temp, test, count;
unsigned char b1_flag, b2_flag, b3_flag, dls_flag;

const unsigned char LCD_Message[] = " MSP430-169 stk  by OLIMEX Ltd. ";
const unsigned char ERASE_SUCCESS_Message[] = "ERASE SUCCESSFUL";
const unsigned char ERASE_ERR_Message[] = "  ERASE  ERROR  ";
const unsigned char PGM_SUCCESS_Message[] = " PGM SUCCESSFUL ";
const unsigned char PGM_ERROR_Message[] = " PROGRAMM ERROR ";
const unsigned char DALLAS_PRESENT_Message[] = " DALLAS PRESENT ";
const unsigned char SAVE_RS232_Message[] = "SAVE RS232 SIGNS";
const unsigned char DYSPLAY_RS232_Message[] = "DYSPLAY to RS232";
unsigned char WRITE_BUF [BUF_SIZE];
unsigned char READ_BUF [BUF_SIZE];

// Function prototypes

void Delay (unsigned int a);
void Delayx100us(unsigned char b);
void _E(void);
void SEND_CHAR (unsigned char d);
void SEND_CMD (unsigned char e);
void InitLCD(void);
void InitUSART0(void);
void UART_transmit (unsigned char Transmit_Data);
void DAC_ini (void);
void ADC_ini (void);
void Get_ADC(unsigned int a);
void Inactive_Flash(void);
void Write_Data(unsigned char a);
unsigned char Programm_Bytes(unsigned char COL_ADD,                          
                             unsigned char ROW_ADDL,
                             unsigned char ROW_ADDH,                             
                             unsigned char NUMBER);   
unsigned char Read_Data(void);      
void Read_Bytes (unsigned char COL_ADD,                          
                 unsigned char ROW_ADDL,
                 unsigned char ROW_ADDH,                             
                 unsigned char NUMBER);                                                 
unsigned char Erase_Flash (unsigned char BLOCK_ADDL, unsigned char BLOCK_ADDH);

//PULL_PIN is P2.0

void main(void)
{ 
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  
  BCSCTL1 &= ~BIT7;                //XT2-ON
  BCSCTL2 |= BIT3;                 //XT2 is SMCLK
    
//hardware ini

  P1DIR=0xff;

  InitLCD();
  
  count=0;
  b1_flag=0;
  b2_flag=0;
  b3_flag=0;
  dls_flag=0;

  for (i=0; i!=32; i++)  
    {
      SEND_CHAR(LCD_Message[i]);
      if (i==15) SEND_CMD (DD_RAM_ADDR2);
    }  
      
  SEND_CMD(DD_RAM_ADDR);      
    
/*  
  while(1){
    Get_ADC(INCH_4);    
    if (DAC12_0DAT++ > (ADC12MEM0 >> 4)) DAC12_0DAT=0;    
    
    Get_ADC(INCH_5);
    if (DAC12_1DAT++ > (ADC12MEM0 >> 4)) DAC12_1DAT=0;         
    P1OUT ^= BIT0;
  }
*/     

  while(1)                   //Repeat forever  
  {          
        
//----------BUTON 1 -------------------------------------------------------------------    
        
    if ((B1) == 0 && b1_flag == 0)          //if B1 pressed
      {      
        Delay(255);        
        b1_flag=1;        
        LED1_ON;
        
        SEND_CMD(CLR_DISP);
        SEND_CMD(DD_RAM_ADDR);
        for (i=0; i!=16; i++)  SEND_CHAR(SAVE_RS232_Message[i]);                 
                                              
      }    
    if ((B1) != 0 && b1_flag != 0)          //if B1 released
      {           
        Delay(255);
        b1_flag=0;             
        LED1_OFF;
        SEND_CMD(CLR_DISP);
        SEND_CMD(DD_RAM_ADDR);
        for (i=0; i!=16; i++)  SEND_CHAR(DYSPLAY_RS232_Message[i]); 
        
        Read_Bytes(0,0,0,BUF_SIZE);             //read from FLASH
        for (i=0; i != BUF_SIZE; i++)           //send to RS232
          {            
            temp=READ_BUF[i];
            UART_transmit(temp);
          }
      }   
             
//----------BUTON 2 -------------------------------------------------------------------                 
             
    if ((B2) == 0 && b2_flag==0)                //if B2 is pressed
      {      
        Delay(255);        
        b2_flag = 1;                  
        LCD_LIGHT_ON;  
        
        if ((Erase_Flash(0,0) & BIT0) == 0)    //if erase flash is successful 
            { 
              SEND_CMD(CLR_DISP);
              SEND_CMD (DD_RAM_ADDR);
              for (i=0; i != 16; i++)  SEND_CHAR(ERASE_SUCCESS_Message[i]);             
            }           
        else                                  //if erase flash is unsuccessful  
            {       
              SEND_CMD(CLR_DISP);
              SEND_CMD (DD_RAM_ADDR);
              for (i=0; i != 16; i++)  SEND_CHAR(ERASE_ERR_Message[i]);  
            }                                                                                                  
      }    
    if ((B2) != 0 && b2_flag != 0)            //if B2 is pressed
      {
         Delay(255);  
         b2_flag = 0;   
         LCD_LIGHT_OFF;
          
         if ((Programm_Bytes(0,0,0,BUF_SIZE) & BIT0) == 0)    //if program flash is successful
          {
            SEND_CMD(CLR_DISP);
            SEND_CMD (DD_RAM_ADDR);
            for (i=0; i != 16; i++)  SEND_CHAR(PGM_SUCCESS_Message[i]);
          }
         else                                                 //if program flash is unsuccessful
          {
            SEND_CMD(CLR_DISP);
            SEND_CMD (DD_RAM_ADDR);
            for (i=0; i != 16; i++)  SEND_CHAR(PGM_ERROR_Message[i]);
          }                                                      
      }
//----------BUTON 3 -------------------------------------------------------------------    

    if ((B3) == 0)
      {                                                
        Get_ADC(INCH_2);                 //LINE_IN_L
        DAC12_0DAT=ADC12MEM0;
        
        Get_ADC(INCH_3);                 //LINE_IN_R
        DAC12_1DAT=ADC12MEM0;   
        LED2_ON;
      }    
    else
      {              
        LED2_OFF;        
        Get_ADC(INCH_0);                //MIC_L
        DAC12_0DAT=ADC12MEM0;
        
//        Get_ADC(INCH_0);
        Get_ADC(INCH_1);                //MIC_R
        DAC12_1DAT=ADC12MEM0;               
      }  
    
//-----------DALLAS test (Erase flash)----------------------------------
    if ((DALLAS) == 0  && dls_flag==0)
     {
        Delay(255);
        dls_flag=1;          
        SEND_CMD(CLR_DISP);
        SEND_CMD (DD_RAM_ADDR2);
        for (i=0; i != 16; i++)  SEND_CHAR(DALLAS_PRESENT_Message[i]);                       
      } 
    if ((DALLAS) != 0  && dls_flag != 0)  
     {
        Delay(255);
        dls_flag=0;
     }              
  }
}
 
 
  

//---Functions difinitions------------------
 
void Delay (unsigned int a)
{
  unsigned char k;
  for (k=0 ; k != a; ++k);                      //20+a*12 cycles (for 1MHz)
}

void Delayx100us(unsigned char b)
{
  unsigned char j;
  for (j=0; j!=b; ++j) Delay (_100us);
}

void _E(void)
{
        E_HIGH;		    //toggle E for LCD
	_NOP();
	_NOP();
	E_LOW;
}

void SEND_CHAR (unsigned char d)
{
	Delayx100us(5);                 //.5ms	
	temp = d & 0xf0;		//get upper nibble	
	LCD_Data &= 0x0f;
	LCD_Data |= temp;               
	RS_HIGH;        	        //set LCD to data mode
	_E();                           //toggle E for LCD
	temp = d & 0x0f;
	temp = temp << 4;               //get down nibble
	LCD_Data &= 0x0f;
	LCD_Data |= temp;
	RS_HIGH;   	                //set LCD to data mode
	_E();                           //toggle E for LCD
}

void SEND_CMD (unsigned char e)
{
	Delayx100us(10);                //10ms
	temp = e & 0xf0;		//get upper nibble	
	LCD_Data &= 0x0f;
	LCD_Data |= temp;               //send CMD to LCD
	RS_LOW;     	                //set LCD to CMD mode
	_E();                           //toggle E for LCD
	temp = e & 0x0f;
	temp = temp << 4;               //get down nibble
	LCD_Data &= 0x0f;
	LCD_Data |= temp;
	RS_LOW;            	        //set LCD to CMD mode
	_E();                           //toggle E for LCD
}

void InitLCD(void)
{
    RS_LOW;
    Delayx100us(250);                   //Delay 100ms
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
    LCD_Data |= BIT4 | BIT5;            //D7-D4 = 0011
    LCD_Data &= ~BIT6 & ~BIT7;
    _E();                               //toggle E for LCD
    Delayx100us(100);                   //10ms
    _E();                               //toggle E for LCD
    Delayx100us(100);                   //10ms
    _E();                               //toggle E for LCD
    Delayx100us(100);                   //10ms
    LCD_Data &= ~BIT4;                  //D7-D4 = 0010
    _E();                               //toggle E for LCD
    
    SEND_CMD(DISP_ON);
    SEND_CMD(CLR_DISP);   
    SEND_CMD(DD_RAM_ADDR);
}

void InitUSART0(void)
{
    UCTL0 = CHAR;                         // 8-bit character    
    
    UTCTL0 = SSEL1;                       // UCLK = XT2    
    UBR00 = 0x41;                         // 8 000 000/9600
    UBR10 = 0x03;                         //
    UMCTL0 = 0x0;


//    UTCTL0 = SSEL0;                       // UCLK = ACLK
//    UBR00 = 0x03;                         // 32k/9600
//    UBR10 = 0x00;                         //
//    UMCTL0 = 0x4a;                        // 

//    UTCTL0 = SSEL0;                       // UCLK = ACLK
//    UBR00 = 0x06;                         // 32k/4800
//    UBR10 = 0x00;                         //
//    UMCTL0 = 0x6f;                        // 
    
    ME1 |= UTXE0 + URXE0;                 // Enabled USART0 TXD/RXD
    IE1 |= URXIE0;                        // Enabled USART0 RX interrupt
    P3SEL |= 0x30;                        // P3.4 = USART0 TXD, P3.5 = USART0 RXD
    P3DIR |= 0x10;                        // P3.4 output direction
    _EINT();                              // Enable interrupts
}

#pragma vector=UART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  unsigned char t;
  _NOP();
  while ((IFG1 & UTXIFG0) == 0);           // USART0 TX buffer ready?
  t=RXBUF0;
  TXBUF0 = t+1;                           // echo+1 
  
  if ((B1) == 0)                          //if B1 is pressed full buffer
    {
      WRITE_BUF[count++] = t;
      if (count > BUF_SIZE-1) count=0;
    }      
    
}

void UART_transmit (unsigned char Transmit_Data)              //UART1 Transmit Subroutine
{
  i=i;
  while ((IFG1 & UTXIFG0) == 0);            //USART0 TX buffer ready?
  TXBUF0 = Transmit_Data;                   //send data
}  

void DAC_ini (void)
{ 
  DAC12_0CTL = DAC12SREF1 + /*DAC12RES + */ DAC12IR + DAC12AMP_7;                          //Ve REF+, 8-bit resolution 
  DAC12_1CTL = DAC12SREF1 + /*DAC12RES + */ DAC12IR + DAC12AMP_7;                          //Ve REF+, 8-bit resolution
  
//  DAC12_0CTL = DAC12SREF1 + DAC12RES + DAC12IR + DAC12AMP_7;                          //Ve REF+, 8-bit resolution 
//  DAC12_1CTL = DAC12SREF1 + DAC12RES + DAC12IR + DAC12AMP_7;
}

void ADC_ini (void)
{
  ADC12CTL0 = SHT0_0 + ADC12ON;         // Set sampling time, turn on ADC12
  ADC12CTL1 = SHP;                      // Use sampling timer
//ADC12IE = 0x01;                       // Enable interrupt  
  ADC12MCTL0 = SREF_7;                  //VR+ = VeREF+ and VR– = VREF–/VeREF–  
  ADC12CTL0 |= ENC;                     // Conversion enabled
  P6SEL = BIT5 + BIT4 + BIT3 + BIT2 + BIT1 + BIT0;                        // P6.4 ADC option select   
}

void Get_ADC(unsigned int a)
{
  ADC12CTL0 &= ~ENC;                                    //disable convertion  
  ADC12MCTL0 &= 0xfff8;                                 //clear select chanel bits
  ADC12MCTL0 |= a;                                      //select chanel
  ADC12CTL0 |= ENC;                                     //enable convertion
  ADC12CTL0 |= ADC12SC;                                 //Sampling open    
  while ((ADC12CTL1 & ADC12BUSY) != 0); 
}

//-----------------NAND FLASH---------------------------

void Inactive_Flash(void)               //pull flash pins to inactive condition
{
  IO_DIR=INPUT;         //IO is inputs
  _CE_OFF;              //=1
  _RE_OFF;              //=1
  _WE_OFF;              //=1
  ALE_OFF;              //=0
  CLE_OFF;              //=0  
}

void Write_Data(unsigned char a)
{
  IO_DIR=OUTPUT;        //IO is outputs
  _WE_ON;
  OUT_PORT=a;  
  _WE_OFF;              //latch data
}

unsigned char Read_Data(void)
{
  unsigned char f;
  IO_DIR=INPUT;         //IO is inputs
  _RE_ON;
  f=IN_PORT;
  _RE_OFF;              //read data  
  return (f);
}

unsigned char Programm_Bytes(unsigned char COL_ADD,                          
                             unsigned char ROW_ADDL,
                             unsigned char ROW_ADDH,                             
                             unsigned char NUMBER)                             
{       
        unsigned char k, l;        
        Inactive_Flash();        
	CLE_ON;
	_CE_ON;	
	Write_Data(WRITE_PAGE);
	CLE_OFF;
	ALE_ON;
        Write_Data(COL_ADD);
        Write_Data(ROW_ADDL);
        Write_Data(ROW_ADDH);
        ALE_OFF;
        for (k=0; k != NUMBER; k++)
          {
             l=WRITE_BUF[k];
             Write_Data(l);
          }
        CLE_ON;
        Write_Data(WRITE_AKN);          
        while ((R_B) == 0);
        Write_Data(READ_STATUS);
        CLE_OFF;
        l = Read_Data();
        Inactive_Flash();        
        return(l);                
}

void Read_Bytes (unsigned char COL_ADD,                          
                 unsigned char ROW_ADDL,
                 unsigned char ROW_ADDH,                             
                 unsigned char NUMBER)
{
        unsigned char n, r;
        Inactive_Flash();        
        CLE_ON;
        _CE_ON;
        Write_Data(READ_0);
        CLE_OFF;
        ALE_ON;
        Write_Data(COL_ADD);
        Write_Data(ROW_ADDL);
        Write_Data(ROW_ADDH);
        ALE_OFF;
        while ((R_B) == 0);
        for (n=0; n != NUMBER; n++)
          {
             r=Read_Data();
             READ_BUF[n] = r;
          }
        Inactive_Flash();  
}

unsigned char Erase_Flash (unsigned char BLOCK_ADDL, unsigned char BLOCK_ADDH)
{       
        unsigned char m; 
        Inactive_Flash();        
	CLE_ON;
	_CE_ON;	
	Write_Data(ERASE_BLOCK);
	CLE_OFF;
	ALE_ON;
        Write_Data(BLOCK_ADDL);
        Write_Data(BLOCK_ADDH);      
        ALE_OFF;        
        CLE_ON;
        Write_Data(ERASE_AKN);          
        while ((R_B) == 0);
        Write_Data(READ_STATUS);
        CLE_OFF;
        m = Read_Data();
        Inactive_Flash();                
        return (m);
}


