
#include  "PCF8593.h"

void ResetPCF8593(void){
    wpisz_data_i2c(0x00,0x00);
    wpisz_data_i2c(0x01,0x00);
    wpisz_data_i2c(0x02,0x00);
    wpisz_data_i2c(0x03,0x00);
    wpisz_data_i2c(0x04,0x00);
}