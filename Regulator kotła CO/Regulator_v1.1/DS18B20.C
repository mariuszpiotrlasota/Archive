/* Thermometer Connections (At your choice) */

#include  <msp430x12x2.h>
#include  "delay.h"
#include  "ds18b20.h"

#define THERM_PORT P2OUT
#define THERM_DDR P2DIR
#define THERM_PIN P2IN
#define THERM_DQ BIT5
/* Utils */
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW() THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH() THERM_PORT|=(1<<THERM_DQ)


#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xbe
#define THERM_CMD_WSCRATCHPAD 0x4e
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xb8
#define THERM_CMD_RPWRSUPPLY 0xb4
#define THERM_CMD_SEARCHROM 0xf0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xcc
#define THERM_CMD_ALARMSEARCH 0xec



char therm_reset(){
    char i;
    //Pull line low and wait for 480uS
    THERM_LOW();
    THERM_OUTPUT_MODE();
    wait_500us();
    //Release line and wait for 60uS
    THERM_INPUT_MODE();
    wait_40us();
    //Store line value and wait until the completion of 480uS period
    i=(THERM_PIN & (1<<THERM_DQ));
    wait_500us();
    //Return the value read from the presence pulse (0=OK, 1=WRONG)
    return i;
}

void therm_write_bit(char bit){
    //Pull line low for 1uS
    THERM_LOW();
    THERM_OUTPUT_MODE();
    
    //If we want to write 1, release the line (if not will keep low)
    if(bit) THERM_INPUT_MODE();
    //Wait for 60uS and release the line
    wait_40us();
    THERM_INPUT_MODE();
}

char therm_read_bit(void){
    char bit=0;
    //Pull line low for 1uS
    THERM_LOW();
    THERM_OUTPUT_MODE();
    
    //Release line and wait for 14uS
    THERM_INPUT_MODE();
    wait_40us();
    //Read line value
    if(THERM_PIN&(1<<THERM_DQ)) bit=1;
    //Wait for 45uS to end and return read value
    wait_40us();
    return bit;
}

char therm_read_byte(void){
    char i=8, n=0;
    while(i--){
    //Shift one position right and store read value
    n>>=1;
    n|=(therm_read_bit()<<7);
    }
    return n;
}


void therm_write_byte(char byte){
    char i=8;
    while(i--){
    //Write actual bit and shift one position right to make 
    //the next bit ready
    therm_write_bit(byte&1);
    byte>>=1;
    }
}



#define THERM_DECIMAL_STEPS_12BIT 625 //.0625 

void therm_read_temperature(char *buffer){
    // Buffer length must be at least 12bytes long! ["+XXX.XXXX C"]
    char temperature[2];
    char digit;
    int decimal;
    //Reset, skip ROM and start temperature conversion
    therm_reset();
    therm_write_byte(THERM_CMD_SKIPROM);
    therm_write_byte(THERM_CMD_CONVERTTEMP);
    //Wait until conversion is complete
    while(therm_read_bit());
    //Reset, skip ROM and send command to read Scratchpad
    therm_reset();
    therm_write_byte(THERM_CMD_SKIPROM);
    therm_write_byte(THERM_CMD_RSCRATCHPAD);
    //Read Scratchpad (only 2 first bytes)
    temperature[0]=therm_read_byte();
    temperature[1]=therm_read_byte();
    therm_reset();
    //Store temperature integer digits and decimal digits
    digit=temperature[0]>>4;
    digit|=(temperature[1]&0x7)<<4;
    //Store decimal digits
    decimal=temperature[0]&0xf;
    decimal*=THERM_DECIMAL_STEPS_12BIT;
    //Format temperature into a string [+XXX.XXXX C]
   // sprintf(buffer, "%+d.%04u C", digit, decimal);
}


