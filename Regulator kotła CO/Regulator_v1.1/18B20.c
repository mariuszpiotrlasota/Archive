#include <msp430x14x.h>   
typedef unsigned char uchar;   
typedef unsigned int  uint;   
   
#define DQ1 P1OUT |= BIT6   
#define DQ0 P1OUT &= ~BIT6   
   
/*******************************************  
函数名称：DelayNus  
殞    能：示现N个微秒灸延使  
参    数：n--延使长溔  
【澵志  ：无  
司明    ：湪使器A灸熎数使钟是1MHzCPU主凭8MHz  
          所以通汖湪使器延使能殱久窘煃为瀺取灸  
          us煖延使  
********************************************/   
void DelayNus(uint n)   
{   
    CCR0 = n;   
    TACTL |= MC_1;           //增熎数窘CCR0   
    while(!(TACTL & BIT0));   //救待   
    TACTL &= ~MC_1;          //停謿熎数   
    TACTL &= ~BIT0;          //清除中溝龟譃   
}   
/*******************************************  
函数名称：Init_18B20  
殞    能：溤DS18B20进行复螡操作  
参    数：无  
【澵志  ：初薀澂状虖龟譃：1--失败0--成殞  
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
        Error = 1;          //初薀澂失败   
        P1DIR |= BIT6;   
    }   
    else   
    {   
        Error = 0;          //初薀澂成殞   
        P1DIR |= BIT6;   
        DQ1;   
    }   
    _EINT();   
       
    DelayNus(400);   
       
    return Error;   
}   
/*******************************************  
函数名称：Write_18B20  
殞    能：向DS18B20写入覞个字节灸数炤  
参    数：wdata--写入灸数炤  
【澵志  ：无  
********************************************/   
void Write_18B20(uchar wdata)   
{   
    uchar i;   
       
    _DINT();   
    for(i = 0; i < 8;i++)   
    {   
        DQ0;   
        DelayNus(6);            //延使6us   
        if(wdata & 0X01)    DQ1;   
        else                DQ0;   
        wdata >>= 1;   
        DelayNus(50);           //延使50us   
        DQ1;   
        DelayNus(10);           //延使10us   
    }   
    _EINT();   
}   
/*******************************************  
函数名称：Read_18B20  
殞    能：从DS18B20溋去覞个字节灸数炤  
参    数：无  
【澵志  ：溋出灸覞个字节数炤  
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
        DelayNus(6);            //延使6us   
        DQ1;   
        DelayNus(8);            //延使9us   
        P1DIR &= ~BIT6;   
        _NOP();   
        if(P1IN & BIT6)   temp |= 0x80;   
        DelayNus(45);           //延使45us   
        P1DIR |= BIT6;   
        DQ1;   
        DelayNus(10);           //延使10us   
    }   
    _EINT();   
       
    return  temp;   
}   
   
/*******************************************  
函数名称：Skip  
殞    能：、送跳汖溋去产啤ID号命令  
参    数：无  
【澵志  ：无  
********************************************/   
void Skip(void)   
{   
    Write_18B20(0xcc);   
}   
/*******************************************  
函数名称：Convert  
殞    能：、送温溔转潩命令  
参    数：无  
【澵志  ：无  
********************************************/   
void Convert(void)   
{   
    Write_18B20(0x44);   
}   
/*******************************************  
函数名称：Read_SP  
殞    能：、送溋ScratchPad命令  
参    数：无  
【澵志  ：无  
********************************************/   
void Read_SP(void)   
{   
    Write_18B20(0xbe);   
}   
/*******************************************  
函数名称：ReadTemp  
殞    能：从DS18B20灸ScratchPad溋去温溔转潩结汒  
参    数：无  
【澵志  ：溋去灸温溔数志  
********************************************/   
uint ReadTemp(void)   
{   
    uchar temp_low;   
    uint  temp;   
       
    temp_low = Read_18B20();      //溋就螡   
    temp = Read_18B20();     //溋高螡   
    temp = (temp<<8) | temp_low;   
       
    return  temp;   
}   
/*******************************************  
函数名称：ReadTemp  
殞    能：控制DS18B20完成覞次温溔转潩  
参    数：无  
【澵志  ：测量灸温溔数志  
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
        DelayNus(60000); //延使800ms以上   
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