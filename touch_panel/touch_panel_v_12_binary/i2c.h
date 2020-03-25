
#define BIT_SDA BIT2
#define BIT_SCL BIT3
#define SDA     2
#define SCL    3
#define I2CDir P3DIR
#define I2COut P3OUT
#define I2CIn  P3IN

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

//******************************************************************************
//             FUNKCJA DLA I2C
//******************************************************************************

void init_i2c(void);
void wait_I2C(void);
void delay_I2C(void);


// funkcja generujaca sygna- start
void i2cstart(void);

// funkcja generujaca sygna- stop
void i2cstop(void);

// funkcja wysy-aj¦ca bajt na szynà I2C
void i2cwrite(unsigned char x);

// funkcja odczytujaca bajt z szyny I2C
unsigned char i2cread(void);
void wpisz_data_i2c(char adres, char dana1);

void czytaj_data_i2c(char adres1);
        




