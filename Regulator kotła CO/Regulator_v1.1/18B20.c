#include <msp430x14x.h>   
typedef unsigned char uchar;   
typedef unsigned int  uint;   
   
#define DQ1 P1OUT |= BIT6   
#define DQ0 P1OUT &= ~BIT6   
   
/*******************************************  
�������ƣ�DelayNus  
��    �ܣ�ʾ��N��΢�����ʹ  
��    ����n--��ʹ����  
����־  ����  
˾��    ����ʹ��A�ğ���ʹ����1MHz��CPU��ƾ8MHz  
          ����ͨ����ʹ����ʹ�ܚ��þ���Ϊ��ȡ��  
          us����ʹ  
********************************************/   
void DelayNus(uint n)   
{   
    CCR0 = n;   
    TACTL |= MC_1;           //��������CCR0   
    while(!(TACTL & BIT0));   //�ȴ�   
    TACTL &= ~MC_1;          //֚ͣ����   
    TACTL &= ~BIT0;          //����МϹ�֞   
}   
/*******************************************  
�������ƣ�Init_18B20  
��    �ܣ���DS18B20���и�Ν����  
��    ������  
����־  ����ʟ��״̏��֞��1--ʧ�ܣ�0--�ɚ�  
********************************************/   
uchar Init_18B20(void)   
{   
    uchar Error;   
      
    _DINT();   
    DQ0;   
    DelayNus(500);   
    DQ1;   
    DelayNus(55);   
    P1DIR &=~ BIT6;   
    _NOP();   
    if(P1IN & BIT6)         
    {   
        Error = 1;          //��ʟ��ʧ��   
        P1DIR |= BIT6;   
    }   
    else   
    {   
        Error = 0;          //��ʟ���ɚ�   
        P1DIR |= BIT6;   
        DQ1;   
    }   
    _EINT();   
       
    DelayNus(400);   
       
    return Error;   
}   
/*******************************************  
�������ƣ�Write_18B20  
��    �ܣ���DS18B20д��ҝ���ֽھ�����  
��    ����wdata--д�������  
����־  ����  
********************************************/   
void Write_18B20(uchar wdata)   
{   
    uchar i;   
       
    _DINT();   
    for(i = 0; i < 8;i++)   
    {   
        DQ0;   
        DelayNus(6);            //��ʹ6us   
        if(wdata & 0X01)    DQ1;   
        else                DQ0;   
        wdata >>= 1;   
        DelayNus(50);           //��ʹ50us   
        DQ1;   
        DelayNus(10);           //��ʹ10us   
    }   
    _EINT();   
}   
/*******************************************  
�������ƣ�Read_18B20  
��    �ܣ���DS18B20��ȥҝ���ֽھ�����  
��    ������  
����־  ��������ҝ���ֽ�����  
********************************************/   
uchar Read_18B20(void)   
{   
    uchar i;   
    uchar temp = 0;   
       
    _DINT();   
    for(i = 0;i < 8;i++)   
    {   
        temp >>= 1;   
        DQ0;   
        DelayNus(6);            //��ʹ6us   
        DQ1;   
        DelayNus(8);            //��ʹ9us   
        P1DIR &= ~BIT6;   
        _NOP();   
        if(P1IN & BIT6)   temp |= 0x80;   
        DelayNus(45);           //��ʹ45us   
        P1DIR |= BIT6;   
        DQ1;   
        DelayNus(10);           //��ʹ10us   
    }   
    _EINT();   
       
    return  temp;   
}   
   
/*******************************************  
�������ƣ�Skip  
��    �ܣ�����������ȥ��ơID������  
��    ������  
����־  ����  
********************************************/   
void Skip(void)   
{   
    Write_18B20(0xcc);   
}   
/*******************************************  
�������ƣ�Convert  
��    �ܣ�������ת������  
��    ������  
����־  ����  
********************************************/   
void Convert(void)   
{   
    Write_18B20(0x44);   
}   
/*******************************************  
�������ƣ�Read_SP  
��    �ܣ����͜�ScratchPad����  
��    ������  
����־  ����  
********************************************/   
void Read_SP(void)   
{   
    Write_18B20(0xbe);   
}   
/*******************************************  
�������ƣ�ReadTemp  
��    �ܣ���DS18B20��ScratchPad��ȥ��ת�����  
��    ������  
����־  ����ȥ������־  
********************************************/   
uint ReadTemp(void)   
{   
    uchar temp_low;   
    uint  temp;   
       
    temp_low = Read_18B20();      //����Ν   
    temp = Read_18B20();     //����Ν   
    temp = (temp<<8) | temp_low;   
       
    return  temp;   
}   
/*******************************************  
�������ƣ�ReadTemp  
��    �ܣ�����DS18B20���ҝ����ת��  
��    ������  
����־  ������������־  
********************************************/   
uint Do1Convert(void)   
{   
    uchar i;    
       
    do   
    {   
        i = Init_18B20();   
    }   
    while(i);   
    Skip();   
    Convert();   
    for(i = 20;i > 0;i--)     
        DelayNus(60000); //��ʹ800ms����   
    do   
    {   
        i = Init_18B20();   
    }   
    while(i);   
    Skip();   
    Read_SP();   
    return ReadTemp();   
}   
</msp430x14x.h>  