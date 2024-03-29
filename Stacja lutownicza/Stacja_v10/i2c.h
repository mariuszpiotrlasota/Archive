

#ifndef I2C_h
#define I2C_h


/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE 
-------------------------------------------------------------------------------------------------------------------------*/
#define BIT_SDA BIT0
#define BIT_SCL BIT3
#define SDA     0
#define SCL     3
#define I2CDir P1DIR
#define I2COut P1OUT
#define I2CIn  P1IN



#define SET_SDA I2COut |= (1 << SDA)
#define CLR_SDA I2COut &= ~(1 << SDA)
//
#define SET_SCL I2COut |= (1 << SCL)
#define CLR_SCL I2COut &= ~(1 << SCL)
//
#define SDA_OUT I2CDir |= (1 << SDA)
#define SDA_IN  I2CDir &= ~(1 << SDA)
//
#define SCL_OUT I2CDir |= (1 << SCL)
#define SCL_IN  I2CDir &= ~(1 << SDA)
//
#define GET_SDA (I2CIn & (1 << SDA))
//
#define ACK 0
#define NOACK 1

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE STRUKTUR
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE ZMIENNYCH
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
			DEFINICJE ZMIENNYCH WSPOLDZIELONYCH
-------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------
			DEKLARACJE PROCEDUR
-------------------------------------------------------------------------------------------------------------------------*/

void init_i2c(void);

void wait_I2C(void);


void delay_I2C(void);
void i2cstart(void);
void i2cstop(void);
void i2cwrite(unsigned char x);
unsigned char i2cread(void);

void wpisz_data_i2c(char adres, char dana1);


unsigned char czytaj_data_i2c(char adres1);


#endif





