#ifndef lcd_4b_h
#define lcd_4b_h



#include  <msp430x13x.h>


/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE 
-------------------------------------------------------------------------------------------------------------------------*/
/*
#define D8    BIT7//P1,7
#define D7    BIT6//P1,6
#define D6    BIT5//P1,5
#define D5    BIT4//P1.4
#define E     BIT2//P1,2
#define RS    BIT1//P1,1
*/

#define LCD_PORT  P1OUT                              //port P1
#define LCD_EN    BIT2                               //port P2.4  
#define LCD_RS    BIT1                               //port P2.3


#define		DISP_ON			0x0c	        //LCD control constants
#define		DISP_OFF		0x08	        //
#define		CLR_DISP		0x01    	//
#define		CUR_HOME		0x02	        //
#define		ENTRY_INC		0x06            //
#define		DD_RAM_ADDR		0x80	        //
#define		DD_RAM_ADDR2		0xc0	        //
#define		DD_RAM_ADDR3		0x28	        //
#define		CG_RAM_ADDR		0x40	        //

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
void           lcd_send_nibble    (unsigned char nn ) ;
void           lcd_send_byte      (unsigned char address,unsigned char n ) ;
void           lcd_init           (void) ;
void           lcd_putc           (unsigned char c) ;
void           lcd_putstr         (unsigned char *p)  ;
void           lcd_cls            (void);
void           lcd_poz            (unsigned char Y, unsigned char X);
void           lcd_putc2          (unsigned char c);
void           lcd_putstr_goto    (unsigned char *p, unsigned char Y, unsigned char X );
void           lcd_znak           (unsigned char znak);

#endif





