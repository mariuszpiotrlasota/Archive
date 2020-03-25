
void odebranie(void)
{
	  int val=0; 

	  val=bufor[0];		
	  IOCLR1 = 0x00FF0000;						// wy³¹cz diody
      IOSET1 = 0x00FF0000 & (val << 16);		// 8 bardziej znacz¹cych bitów
	  val=0;


}

void akcelerometr(void)
{
	   int i=0;
		int chan1,chan2;
		int dana_x=0,dana_y=0;
		int dana=0;
		chan1 = get_ad_chanel(1);		
		chan2 = get_ad_chanel(2);
		dana_x=	(chan1 >> 2); //do warotsci 8bitowej				to bede wysylal
	    dana_y=	(chan2 >> 2); //do warotsci 8bitowej
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

//**********************************************init_AD
