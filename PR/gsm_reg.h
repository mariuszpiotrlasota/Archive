
//teksty uzywane w programie wyswieltacza
unsigned char LCD_MENU[8]=     "MENU  <>";
unsigned char LCD_SIEC[8]=     "1 SIEC";
unsigned char LCD_GSM_on[8]=    "1 GSM ON";
unsigned char LCD_GSM_off[8]=   "1 GSMOFF"; 
unsigned char LCD_NUMER[8]= "2 NUMER";
unsigned char LCD_GSM[8]=    "3 SET <<";
unsigned char LCD_SMS[8]=    "4 >> SMS";
unsigned char LCD_MEM[8]=    "4 CLRMEM";
unsigned char LCD_TEST[8]=         "2   TEST";
unsigned char LCD_GSM_on_off[8]=   "5 ON/OFF";

//inicjacja GSM'u
unsigned char CPIN[] = "AT+CPIN=1643";	//wprowadzenie numeru PIN
unsigned char CHOOSEMEM[] = "AT+CPMS=";	//wyb°r domyùlnej lokalizacji 
unsigned char ECHOOFF[] = "ATE0";			//wy-¶czenie echa komendy AT
unsigned char SETNOTICE[] = "AT+CNMI=1,1,0,2";
unsigned char NOTICE[] = "+CMTI:";		//powiadomienie o nowym SMS-ie
unsigned char SMSDEL[] = "AT+CMGD=";		//usuni‡cie SMS-a z pami‡ci
unsigned char SMSREAD[] = "AT+CMGR=";	//odczyt SMS-a z pami‡ci
unsigned char SMSSEND[] = "AT+CMGS=";	//wys-anie SMS-a
/*Parametry wysy≥ania SMS*/
unsigned char LOSCA[] = "07";					//D-ugoùä numeru SCA+1
unsigned char TOSCA[] = "91";					//Typ numeracji SCA
char CENTRALA_idea[] = "8405210077F7";	//Centrum Us-ug (tu dla sieci													//Plus GSM)
unsigned char FO = 0x11;							//Pierwszy oktet dla wysy-anego SMS-a
unsigned char MR[] = "00";						//Numer odniesienia dla komunikatu
unsigned char LODA[] = "0B";					//Liczba cyfr numeru telefonu 
													//ODBIORCA
unsigned char TODA[] = "91";					//Numeracja mi‡dzynarodowa dla CU 
char NUMER_Tato[] =     "8415539422F1";
char NUMER_DOCELOWY[] = "8487438222F4"; 	//Odbiorca wiadomoùci SMS
unsigned char PID[] = "00";						//Identyfikator protoko-u (tekst)
unsigned char DCS[] = "00";						//Schemat kodowania
unsigned char SCTS[] = "8F";					//Okres wa¨noùci 12 godzin


