#ifndef _ds18b20_h
#define _ds18b20_h

char therm_reset();

void therm_write_bit(char bit);

char therm_read_bit(void);

char therm_read_byte(void);


void therm_write_byte(char byte);
void therm_read_temperature(char *buffer);

#endif