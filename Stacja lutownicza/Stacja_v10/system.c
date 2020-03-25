
#include "delay.h"
#include "system.h"

/**************************************************************************************************************************
procedura:
procesorWTDStop 	: zwraca adres docelowy ramki
**************************************************************************************************************************/
void procesorClkSetup(void){
  //SR=BIT1;
  DCOCTL &=~(0XFF);
  BCSCTL1 =BIT7;
  BCSCTL1 =BIT7|BIT3;
}//procesorClkSetup



  
/**************************************************************************************************************************
procedura:
procesorClk32k		: 
**************************************************************************************************************************/
void procesorSetLowMCLK(void){
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
}//procesorSetLowMCLK


void procesorSetInternalCLK(void){
  DCOCTL=0XFF;    //OSILLATOT
  BCSCTL1=BIT1+BIT2+BIT3;
}//procesorSetInternalCLK
  
/**************************************************************************************************************************
procedura:
procesorClkSetup_CPG		: 
**************************************************************************************************************************/
void procesorClkSetup_CPG(void)
{
  volatile unsigned short m_usIndex;
	IFG1	= 0;
//	__bic_SR_register(LPM4_bits);

//	BCSCTL1	= (DIVA0 | DIVA1 | XT2OFF);
	BCSCTL1	= (XT2OFF);

	BCSCTL2 = 0;
//	BCSCTL2 = (SELM0 | SELM1);

	__bic_SR_register(LPM4_bits);

	m_usIndex	= 1000;

	do
	{
		IFG1	&= ~OFIFG;

		procesorDelay(100);

		m_usIndex--;

	}while(IFG1 & OFIFG);

	IE1	&= ~OFIE;
}//procesorClkSetup_CPG

/**************************************************************************************************************************
procedura:
procesorWTDStop 	: zwraca adres docelowy ramki
**************************************************************************************************************************/
void procesorWTDStop(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
}//procesorWTDStop


/**************************************************************************************************************************
procedura:
initIO		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void procesorInitIo(void)
{

}//procesorInitIo


/**************************************************************************************************************************
procedura:
Led_test		: zwraca adres docelowy ramki
**************************************************************************************************************************/
void led_test(void)
{

}

