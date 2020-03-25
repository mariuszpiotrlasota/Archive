#define BIT_SDA BIT1
#define BIT_SCL BIT0
#define SDA     1
#define SCL     0
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