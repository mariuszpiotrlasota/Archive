#define BIT_SDA BIT1
#define BIT_SCL BIT2
#define SDA     1
#define SCL     2
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