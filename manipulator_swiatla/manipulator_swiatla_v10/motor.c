#include  <msp430x16x.h>
#include  "wait.h"

#define EN_1  BIT5
#define IN1_1 BIT4
#define IN2_1 BIT6
#define D_1   BIT7

#define EN_2  BIT1
#define IN1_2 BIT0
#define IN2_2 BIT2
#define D_2   BIT3

#define EN_3  BIT5
#define IN1_3 BIT4
#define IN2_3 BIT6
#define D_3   BIT7

#define EN_4  BIT1
#define IN1_4 BIT0
#define IN2_4 BIT2
#define D_4   BIT3

#define EN_5  BIT7
#define IN1_5 BIT6
#define IN2_5 BIT0
#define D_5   BIT1

//------------------------------------------------------------------------------
//                               podstawa 
//------------------------------------------------------------------------------
void m2_down(void)
{
  P3OUT=EN_5+IN1_5;
}

void m2_up(void)
{
  P3OUT=EN_5;
  P4OUT=IN2_5;
}

//------------------------------------------------------------------------------
//                               chytak
//------------------------------------------------------------------------------
void m5_open(void)
{
  P2OUT=EN_2+IN1_2;
}

void m5_close(void)
{
  P2OUT=EN_2+IN2_2;
}


//------------------------------------------------------------------------------
//                             pod chwytak
//------------------------------------------------------------------------------
void m4_up(void)
{
  P2OUT=EN_3+IN1_3;
}

void m4_down(void)
{
  P2OUT=EN_3+IN2_3;
}


//------------------------------------------------------------------------------
//                               pod pod chwytak
//------------------------------------------------------------------------------
void m3_up(void)
{
  P3OUT=EN_4+IN1_4;
}

void m3_down(void)
{
  P3OUT=EN_4+IN2_4;
}


//------------------------------------------------------------------------------
//                               przy podstawie
//------------------------------------------------------------------------------
void m1_right(void)
{
  P1OUT=EN_1+IN1_1;
}

void m1_left(void)
{
  P1OUT=EN_1+IN2_1;
}


//------------------------------------------------------------------------------
//                               CLEAR M
//------------------------------------------------------------------------------
void m_clear(void)
{
    P1OUT=0x00;
    P2OUT=0x00;
    P3OUT=0x00;
    P4OUT=0x00;
}

 

void test_m(void)
{
 
  m_clear();wait_long();
  m2_down();
  
  wait_long();
  m_clear();wait_long();
  m2_up();
  
  wait_long();
  m_clear();wait_long();
  m4_up();
  
  wait_long();
  m_clear();wait_long();
  m4_down();
  
  wait_long();
  m_clear();wait_long();
  m3_up();
  
  wait_long();
  m_clear();wait_long();
  m3_down();
  
  wait_long();
  m_clear();wait_long();
  m1_right();
  
  wait_long();
  m_clear();wait_long();
  m1_left();
    
  wait_long();
  m_clear();wait_long();
  m5_open();
    
  wait_long();
  m_clear();wait_long();
  m5_close();
    wait_long();
  m_clear();wait_long();

}