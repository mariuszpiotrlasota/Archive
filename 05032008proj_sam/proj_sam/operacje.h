#define AD0CR_CLKDIV_BIT 8
#define AD0CR_PDN_BIT 21
#define AD0CR_SEL_MASK 0xFF
#define AD0CR_START_BIT 24
#define AD0GDR_DONE 0x80000000

#define P028_ADC01_SEL (1<<24)
#define P029_ADC02_SEL (1<<26)


int get_ad(int kanal);
int get_ad_chanel(int Chan);
void init_ad(void);
void odebranie(void);
void akcelerometr(void);