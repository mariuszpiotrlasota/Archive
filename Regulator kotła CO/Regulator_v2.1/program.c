#include "program.h"
#include "i2c.h"

  unsigned char G_Tkotla_reguRegul; //wartoœci 
  unsigned char G_Twody_reguRegul;  //wartoœci 
  unsigned char G_Przedmuch;  //wartoœci 
  unsigned char G_Tkotla_reguWygasz;
    
    
  unsigned char G_licz_przedmuch=0;
  unsigned char temp_regul_dzies,temp_regul_jedno;  
  
  unsigned char _set=0;  
  unsigned char G_Tkotla_reguAktualna=0,G_Twody_reguAktualna=0;
  unsigned char set_min_licz=0, set_min_dzies, set_min_jedn;
  unsigned char G_minAktualna, G_sekAktualna, G_godzAktualna;
/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void set_main_text(void)
{

  lcd_putstr_goto("Tkotla",1,1); 
  lcd_putstr_goto("Twody", 2,1);
  lcd_putstr_goto("Totocz",3,1);
  lcd_putstr_goto("    ",3,17);
  lcd_putstr_goto("00:00:00", 4,1);

  
  lcd_putstr_goto("--",1,19);  
  lcd_putstr_goto("--",2,19);
  
  lcd_putstr_goto("/",1,18); 
  lcd_putstr_goto("/",2,18); 
}//set_main_text




  unsigned char temp_msb,temp_lsb;
  unsigned char  digit_1, digit_2, digit_3; 
  float temp;
  unsigned short int temp_c;
  unsigned char Tkotla_regu=0,Twody_regu=0;
  unsigned char Tkotla_regu_wygasz=0;

  unsigned char Tcalkowita;
  unsigned short int rozdziel=625,Tulamek;
/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void setTempChanOnLCD(unsigned char channel) 
{
           WireReset(channel);wait_160us();
           WireWriteByte(0xcc,channel);wait_100us();
           WireWriteByte(0x44,channel);wait_100us(); 
           WireReset(channel);wait_160us(); 
           WireWriteByte(0xcc,channel);wait_100us();
           WireWriteByte(0xbe,channel);wait_100us();
           wait_160us();
           
        
           temp_lsb=WireReadByte(channel);//lsb
           temp_msb=WireReadByte(channel);//msb

           
           //*******************************************temp. ujenme
           if((temp_msb & 0x80)!=0){
                temp_c=(temp_msb<<8)+(temp_lsb);
                Tcalkowita=(temp_c>>4);
                Tcalkowita=0xff-Tcalkowita;
                
                Tulamek=temp_c & 0x0f;//lsb -25
                Tulamek=(0x0f-Tulamek+1)*rozdziel;
                Tulamek=Tulamek/1000;
                
                 digit_1= Tcalkowita/10;
                 digit_2= (Tcalkowita)-(digit_1*10);
                 digit_3= Tulamek;
           }
           
           
           
             //*******************************************temp. dodatnie
         else{
                 temp_c=(temp_msb<<8)+temp_lsb;
                 Tcalkowita=(temp_c>>4);
                 Tulamek=(temp_c & 0x0F)*rozdziel;
                 Tulamek=Tulamek/1000;
                 
                 
                 digit_1= Tcalkowita/10;
                 digit_2= (Tcalkowita)-(digit_1*10);
                 digit_3= Tulamek;
           }
           
           
           
           
           //obliczenia temp. do procesu regulacji
               switch(channel){
                  case 1: lcd_poz(1,9); G_Tkotla_reguAktualna=((digit_1*10) + digit_2); break;
                  case 2: lcd_poz(2,9); G_Twody_reguAktualna=((digit_1*10) + digit_2); break;
                  case 3: lcd_poz(3,9);  break;
               }
               
           
           
           //wyœwietl na LCD temperatury
               if(temp_msb & 0x80){lcd_putc('-');}
               else{lcd_putc('+');}
           
               lcd_znak(digit_1);
               lcd_znak(digit_2);
               lcd_putc('.');
               lcd_znak(digit_3);
               
              switch(channel){
                  case 1: lcd_putstr_goto("C", 1,14);    break;
                  case 2: lcd_putstr_goto("C", 2,14);    break;
                  case 3: lcd_putstr_goto("C", 3,14);    break;
               }
               
               
                  
}

/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void SetKotlaWentylatorOFF(void){
     lcd_putstr_goto("-", 3,18);    
     OutDriveOUT&=~(OutDrive1);     
}

/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void SetKotlaWentylatorON(void){
     lcd_putstr_goto("W", 3,18);
       OutDriveOUT|=(OutDrive1);
}
       
/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void SetPompaOFF(void){
     lcd_putstr_goto("-", 3,19);  
     OutDriveOUT&=~(OutDrive2);
}

/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void SetPompaON(void){
     lcd_putstr_goto("P", 3,19);    
     OutDriveOUT|=(OutDrive2);     
}
       


/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void RegulacjaWody(void){
         
        temp_regul_dzies=G_Twody_reguRegul/10;     //dziesietna
        temp_regul_jedno=G_Twody_reguRegul-temp_regul_dzies*10;    //jednost
          
          lcd_putstr_goto("", 2,19); lcd_znak(temp_regul_dzies);
          lcd_putstr_goto("", 2,20); lcd_znak(temp_regul_jedno);       
  
 if(G_Twody_reguAktualna<G_Twody_reguRegul)
    {SetPompaOFF();}
 else 
    {SetPompaON();}      
}
       
/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void set_pause_przedmuch(unsigned char pauze){
  
  switch(G_licz_przedmuch){
  
  case 0:  SetKotlaWentylatorON(); break;
  case 10: SetKotlaWentylatorOFF();  break;

  }
  
  G_licz_przedmuch++;
    if(G_licz_przedmuch>(G_Przedmuch*pauze+10)){G_licz_przedmuch=0;}   
     

   
  
}


/**************************************************************************************************************************
procedura:
RegulacjaKotlaWygaszanie 	: 
**************************************************************************************************************************/
void RegulacjaKotlaWygaszanie(void){

        temp_regul_dzies=G_Tkotla_reguWygasz/10;     //dziesietna
        temp_regul_jedno=G_Tkotla_reguWygasz-temp_regul_dzies*10;    //jednost
          
          lcd_putstr_goto("", 1,16); lcd_znak(temp_regul_dzies);
          lcd_putstr_goto("", 1,17); lcd_znak(temp_regul_jedno);
  
}


/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void RegulacjaKotla(void){

    
        temp_regul_dzies=G_Tkotla_reguRegul/10;     //dziesietna
        temp_regul_jedno=G_Tkotla_reguRegul-temp_regul_dzies*10;    //jednost
          
          lcd_putstr_goto("", 1,19); lcd_znak(temp_regul_dzies);
          lcd_putstr_goto("", 1,20); lcd_znak(temp_regul_jedno);
      
  
if(G_Tkotla_reguAktualna<G_Tkotla_reguRegul) //tem. wiêksza od regulacji
{
            if(G_Tkotla_reguAktualna>=G_Tkotla_reguWygasz){SetKotlaWentylatorON();}
            else {SetKotlaWentylatorOFF();}
              
}


 else 
    {SetKotlaWentylatorOFF();}
  
}

   

/**************************************************************************************************************************
procedura:
RegulacjaPrzedmuch 	: 
**************************************************************************************************************************/
void RegulacjaPrzedmuch(void){
 
   //wyœwietl aktualny przedmuch
   switch(G_Przedmuch){
            case 0: lcd_putstr_goto("    ", 3,17); wait_long();break;
            case 1: lcd_putstr_goto(">   ", 3,17); wait_long();break; 
            case 2: lcd_putstr_goto(">>  ", 3,17); wait_long();break; 
            case 3: lcd_putstr_goto(">>> ", 3,17); wait_long();break; 
            case 4: lcd_putstr_goto(">>>>", 3,17); wait_long();break; 
            
          }
   
}


/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void StanPRACA(void){
              //lcd_putstr_goto("s:PRACA", 4,14);
              setTempChanOnLCD(1);
              wait_short();
              setTempChanOnLCD(2);
              wait_short();
              setTempChanOnLCD(3);
              wait_short(); 
              RegulacjaKotla();
              RegulacjaKotlaWygaszanie();
              RegulacjaWody();
              //RegulacjaPrzedmuch();
              
}
              
  

/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void StanROZPAL(void){
              //lcd_putstr_goto("s:ROZPA", 4,14);
              setTempChanOnLCD(1);
              wait_short();
              setTempChanOnLCD(2);
              wait_short();
              setTempChanOnLCD(3);
              wait_short();  
    
}
   


/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void StanWYGASZ(void){
              //lcd_putstr_goto("s:WYGAS", 4,14);
              setTempChanOnLCD(1);
              wait_short();
              setTempChanOnLCD(2);
              wait_short();
              setTempChanOnLCD(3);
              wait_short();
     
}
              



/**************************************************************************************************************************
procedura:
setTemWygaszKotlaOnLCD 	: 
**************************************************************************************************************************/
void setTemWygaszKotlaOnLCD(void){

          lcd_putstr_goto("p", 4,11);
          lcd_putstr_goto("--", 1,19);
          lcd_putstr_goto("--", 2,19);
          
          temp_regul_dzies=Tkotla_regu_wygasz/10;     //dziesietna
          temp_regul_jedno=Tkotla_regu_wygasz-temp_regul_dzies*10;    //jednost
         
  
            
               
          
          lcd_putstr_goto("", 1,16); lcd_znak(temp_regul_dzies);
          lcd_putstr_goto("", 1,17); lcd_znak(temp_regul_jedno);
          if(S1IN==0){wait_long();Tkotla_regu_wygasz++;  }
          if(S2IN==0){wait_long();Tkotla_regu_wygasz--;  }
          if(Tkotla_regu_wygasz>99){Tkotla_regu_wygasz=0;}if(Tkotla_regu_wygasz<0){Tkotla_regu_wygasz=99;}
          
          G_Tkotla_reguWygasz=((temp_regul_dzies*10) + (temp_regul_jedno));
          

}


/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void setTemRegKotlaOnLCD(void){
        
          
          lcd_putstr_goto("p", 4,11);
          lcd_putstr_goto("--", 1,16);
          lcd_putstr_goto("--", 2,19);          
          temp_regul_dzies=Tkotla_regu/10;     //dziesietna
          temp_regul_jedno=Tkotla_regu-temp_regul_dzies*10;    //jednost
         
  
            
               
          
          lcd_putstr_goto("", 1,19); lcd_znak(temp_regul_dzies);
          lcd_putstr_goto("", 1,20); lcd_znak(temp_regul_jedno);
          if(S1IN==0){wait_long();Tkotla_regu++;  }
          if(S2IN==0){wait_long();Tkotla_regu--;  }
          if(Tkotla_regu>99){Tkotla_regu=0;}if(Tkotla_regu<0){Tkotla_regu=99;}
          
          G_Tkotla_reguRegul=((temp_regul_dzies*10) + (temp_regul_jedno));
          

}



/**************************************************************************************************************************
procedura:
set_main_text 	: 
**************************************************************************************************************************/
void setTemRegWodyOnLCD(void){
          lcd_putstr_goto("p", 4,11);
          lcd_putstr_goto("--/--", 1,16);

          temp_regul_dzies=Twody_regu/10;
          temp_regul_jedno=Twody_regu-temp_regul_dzies*10;
          
          lcd_putstr_goto("", 2,19); lcd_znak(temp_regul_dzies);
          lcd_putstr_goto("", 2,20); lcd_znak(temp_regul_jedno);
          if(S1IN==0){wait_long();Twody_regu++;  }
          if(S2IN==0){wait_long();Twody_regu--;  }
            if(Twody_regu>99){Twody_regu=0;}if(Twody_regu<0){Twody_regu=99;}
          
          G_Twody_reguRegul=((temp_regul_dzies*10) + (temp_regul_jedno));
}





/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/

void ShowTimeOnLCD(void){

          //pobierz wartosci
          G_godzAktualna=czytaj_data_i2c(PCF8593_godz);
          G_minAktualna=czytaj_data_i2c(PCF8593_min); 
          G_sekAktualna=czytaj_data_i2c(PCF8593_sek);
          
          
          //sekundy           
          lcd_putstr_goto("", 4,7); lcd_znak((G_sekAktualna & 0xF0)>>4);
          lcd_putstr_goto("", 4,8); lcd_znak(G_sekAktualna & 0x0F);
          
          
          //minuty       
          lcd_putstr_goto(":",4,6);
          lcd_putstr_goto("", 4,4); lcd_znak((G_minAktualna & 0xF0)>>4);
          lcd_putstr_goto("", 4,5); lcd_znak(G_minAktualna & 0x0F);

          //godziny           
          lcd_putstr_goto(":",4,3);
          lcd_putstr_goto("", 4,1); lcd_znak((G_godzAktualna & 0xF0)>>4);
          lcd_putstr_goto("", 4,2); lcd_znak(G_godzAktualna & 0x0F);
          if( ((G_godzAktualna & 0xF0)>>4)>2){                              //warunek na 24h
            if( (G_godzAktualna & 0xF0)>4 ){ wpisz_data_i2c(0x04,0x00);}
           }
}





/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/
unsigned char  set_godz_licz=0, set_godz_dzies, set_godz_jedn;
void SetTimeGodzOnLCD(void){
  
       


          lcd_putstr_goto("p", 4,11);
          lcd_putstr_goto("--/--", 1,16);
          lcd_putstr_goto(":--   ", 4,3);
          
         
          set_godz_dzies=set_godz_licz/10;             //dziesietne
          set_godz_jedn=set_godz_licz-set_godz_dzies*10;    //jednostkowe
          
          lcd_putstr_goto("", 4,1); lcd_znak(set_godz_dzies);
          lcd_putstr_goto("", 4,2); lcd_znak(set_godz_jedn);
    
          if(S1IN==0){wait_long();set_godz_licz++; }
          if(S2IN==0){wait_long();set_godz_licz--;  }
         
          if(set_godz_licz>23){set_godz_licz=0;}
          if(set_godz_licz<0){set_godz_licz=23;}

}



/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/

void SetTimeMinOnLCD(void){

          
          lcd_putstr_goto("p", 4,11);
          lcd_putstr_goto("--/--", 1,16);
          lcd_putstr_goto("   ", 4,6);
          
         
          set_min_dzies = set_min_licz/10;             //dziesietne
          set_min_jedn = set_min_licz - set_min_dzies*10;    //jednostkowe
          
          lcd_putstr_goto("", 4,4); lcd_znak(set_min_dzies);
          lcd_putstr_goto("", 4,5); lcd_znak(set_min_jedn);
    
             
          if(S1IN==0){wait_long();set_min_licz++; }
          if(S2IN==0){wait_long();set_min_licz--;  }   
           
          if(set_min_licz<0){set_min_licz=59;}
          if(set_min_licz>59){set_min_licz=0;}              


}



/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/
void StoreDataOnPCF8593(void){
  wpisz_data_i2c(PCF8593_godz,( (set_godz_dzies<<4) + (set_godz_jedn) ));
  wpisz_data_i2c(PCF8593_min,( (set_min_dzies<<4) + (set_min_jedn) ));
  wpisz_data_i2c(PCF8593_sek,0x00); //zeruj sekundy
  
  wpisz_data_i2c(PCF8593_reg_kotla,G_Tkotla_reguRegul); //wpisz reg. 
  wpisz_data_i2c(PCF8593_reg_wody,G_Twody_reguRegul);  //wpisz reg. 
}


void  SetPrzedmuch(void){
          if(S1IN==0){wait_long();G_Przedmuch++; }
          if(S2IN==0){wait_long();G_Przedmuch--; }  
    
          lcd_putstr_goto("p:PRZED", 4,14);
          lcd_putstr_goto("    ", 3,17); 
          
          switch(G_Przedmuch){
            case 0: lcd_putstr_goto("    ", 3,17); wait_long();break; 
            case 1: lcd_putstr_goto(">   ", 3,17); wait_long();break; 
            case 2: lcd_putstr_goto(">>  ", 3,17); wait_long();break; 
            case 3: lcd_putstr_goto(">>> ", 3,17); wait_long();break; 
            case 4: lcd_putstr_goto(">>>>", 3,17); wait_long();break; 
          }
          if(G_Przedmuch>4){G_Przedmuch=0;}
          if(G_Przedmuch<0){G_Przedmuch=4;}
}






/**************************************************************************************************************************
procedura:
WriteFlashMSP430		: 
**************************************************************************************************************************/
void WriteFlashMSP430 (unsigned char Tkotla_regu, unsigned char Twody_regu, unsigned char Przedmuch, unsigned char Wygaszanie )
{
  char *Flash_ptr;                          // Flash pointer


  Flash_ptr = (char *) 0x1080;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

    *Flash_ptr = Tkotla_regu;                   // 0x1080
    *Flash_ptr++;
    *Flash_ptr = Twody_regu;                    //0x1081
    *Flash_ptr++;
    *Flash_ptr = Przedmuch;                    //0x1082
    *Flash_ptr++;
    *Flash_ptr = Wygaszanie;                    // 

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}//WriteFlashMSP430







/**************************************************************************************************************************
procedura:
ReadFlashMSP430Kotla		: 
**************************************************************************************************************************/

unsigned char ReadFlashMSP430_sector(unsigned char sector)
{
  char *Flash_ptrA;                         // Segment A wskaznik
  unsigned char flash_data; 
  
  Flash_ptrA = (char *) 0x1080;             // Initialize Flash segment A pointer

  for(unsigned int i=0;i<sector;i++){ 
    flash_data = *Flash_ptrA;           // copy value segment A to segment B
  *Flash_ptrA++;  
  }
  
  return(flash_data);
}//ReadFlashMSP430Kotla




/**************************************************************************************************************************
procedura:
ReadFlashMSP430Kotla		: 
**************************************************************************************************************************/
void WriteFlashMSP430Data(void)
{
  WriteFlashMSP430(G_Tkotla_reguRegul,G_Twody_reguRegul,G_Przedmuch,G_Tkotla_reguWygasz);
}



/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/
void ReadFlashMSP430(void){

       G_Tkotla_reguRegul=ReadFlashMSP430_sector(1);
       G_Twody_reguRegul=ReadFlashMSP430_sector(2);
       G_Przedmuch=ReadFlashMSP430_sector(3);
       G_Tkotla_reguWygasz=ReadFlashMSP430_sector(4);
}


/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/
void ShowLogoOnLCD(void){
  
  lcd_putstr_goto("Regulator kotla",2,3); 
  lcd_putstr_goto("__ver: 2.1__",3,5); 
  wait_long2();wait_long2();wait_long2();
  lcd_putstr_goto("                   ",1,1); 
  lcd_putstr_goto("                   ",2,1); 
  lcd_putstr_goto("                   ",3,1); 
  lcd_putstr_goto("                   ",4,1); 
  
}



/**************************************************************************************************************************
procedura:
	: 
**************************************************************************************************************************/
void initDrivePort(void){
  OutDriveDIR|=(OutDrive1+OutDrive2+OutDrive3+OutDrive4+OutDrive5);
  DrivePortOFF();

}

void DrivePortOFF(void){
    OutDriveOUT&=~(OutDrive1+OutDrive2+OutDrive3+OutDrive4+OutDrive5);
}