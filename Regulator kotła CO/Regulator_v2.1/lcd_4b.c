

#include "lcd_4b.h"

/*
#define D8    BIT7//P1,7
#define D7    BIT6//P1,6
#define D6    BIT5//P1,5
#define D5    BIT4//P1.4
#define E     BIT2//P1,2
#define RS    BIT1//P1,1
*/


unsigned int ki;

/**************************************************************************************************************************
procedura:
lcd_send_nibble 	: 
**************************************************************************************************************************/
void lcd_send_nibble(unsigned char nn) {
      P1OUT&=~LCD_EN;                 //LCD_EN = 0;
      P1OUT&=0x0F;
      nn&=0xF0;                       //i wyzeruj mlodsza
      P1OUT|=nn;                      //zapisz do portu  
      _NOP();
      P1OUT|=LCD_EN;                  //LCD_EN= 1;
      _NOP();
      _NOP();
      P1OUT&=~LCD_EN;                 //LCD_EN = 0;
}//lcd_send_nibble

/**************************************************************************************************************************
procedura:
lcd_send_byte 	: 
**************************************************************************************************************************/
void lcd_send_byte(unsigned char address,unsigned char n ) {
char k;
  P1OUT&=~LCD_RS;                     // jesli komenda to zeruj RS
  for(k=0;k<20;k++);                  //czekaj na wyswietlacz  
  if(address==1) P1OUT|=LCD_RS;       
 // _NOP();                             //delay_cycles(1);
  P1OUT&=~LCD_EN;                     //lcd_en = 0;
  lcd_send_nibble(n);                 //wyslij starsza tetrade
  lcd_send_nibble(n<<4);              //wyslij mlodsza tetrade  
}//lcd_send_byte


/**************************************************************************************************************************
procedura:
lcd_init 	: 
**************************************************************************************************************************/
void lcd_init(void){
  char j;
    P1OUT&=0x0F;                       //wyzeruj P1.4-p1.7
    P1DIR|= 0xF0;                      // P1.4 - P1.7 jako wyjscia
    P1OUT&=~LCD_EN;                    //zeruj EN
    P1OUT&=~LCD_RS;                    //zeruj RS
    P1DIR|=LCD_EN;                     //EN i RS jako wyjscia
    P1DIR|=LCD_RS;

    for(ki=0;ki<10000;ki++);            //czekaj ok. 15ms  
    for(j=1;j<=3;++j) {
       lcd_send_nibble(0x30);
       for(ki=0;ki<2000;ki++);         //czekaj ok. 5ms 
    }
    lcd_send_nibble(0x20);
    lcd_send_byte(0,0x28);             //wyswietlacz 2linie  
    lcd_send_byte(0,0x0c);             //lcd on
    lcd_send_byte(0,1);                //clear lcd
    for(ki=0;ki<1000;ki++);            //czekaj ok. 2ms
    lcd_send_byte(0,6);                //entry mode set
}//lcd_init


/**************************************************************************************************************************
procedura:
lcd_putc 	: 
**************************************************************************************************************************/
void lcd_putc(unsigned char c) {
   //switch (c) {
   //  case '\f'   : lcd_send_byte(0,1);          //zeruje wyswietlacz    
   //                for(ki=0;ki<1000;ki++);     //czekaj ok. 2ms
     //              break;
    // case '\b'   : lcd_send_byte(0,0x02);  break; //ustawienie kursora na 1pozycji
    // default     : 
       lcd_send_byte(1,c);    
       //break; //wyswietlenie znaku
  // }
}//lcd_putc




/**************************************************************************************************************************
procedura:
lcd_putstr 	: 
**************************************************************************************************************************/
void lcd_putstr (unsigned char *p)   // wyœlij string przez 
{
  while (*p)
  {
    lcd_putc (*p++);
  }
  //putchar('\n');
}//lcd_putstr


/**************************************************************************************************************************
procedura:
lcd_putstr 	: 
**************************************************************************************************************************/
void lcd_putstr_goto (unsigned char *p, unsigned char Y, unsigned char X )   // wyœlij string przez 
{
  lcd_poz(Y,X);
  while (*p)
  {
    lcd_putc (*p++);
  }

}//lcd_putstr




/**************************************************************************************************************************
procedura:
lcd_cls 	: 
**************************************************************************************************************************/
void lcd_cls (void){
 lcd_send_byte(0,1);  
}//lcd_cls



/**************************************************************************************************************************
procedura:
lcd_putc 	: 
**************************************************************************************************************************/
void lcd_poz(unsigned char Y, unsigned char X){
   unsigned char poz=0; 
   
  switch(Y){
    case 1: poz=poz+X-1; break;
    case 2: if(X>1){poz=poz+X+2;} poz=poz+61; break;
    case 3: poz=poz+20+X-1; break;
    case 4: poz=poz+84+X-1; break;  
  }   
  
  lcd_send_byte(0,0x80+poz);
}//lcd_putc





/**************************************************************************************************************************
procedura:
lcd_putc 	: 
**************************************************************************************************************************/
void lcd_znak(unsigned char znak){
           switch(znak){
            case 0: lcd_putc('0');break;
            case 1: lcd_putc('1');break;
            case 2: lcd_putc('2');break;
            case 3: lcd_putc('3');break;
            case 4: lcd_putc('4');break;
            case 5: lcd_putc('5');break;
            case 6: lcd_putc('6');break;
            case 7: lcd_putc('7');break;
            case 8: lcd_putc('8');break;
            case 9: lcd_putc('9');break;  
           }
}

