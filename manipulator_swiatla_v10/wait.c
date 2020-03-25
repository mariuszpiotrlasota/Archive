//------------------------------------------------------------------------------
//                               WAIT
//------------------------------------------------------------------------------
void wait_short(void)
{
  volatile unsigned int i;
   i = 10000;                              // Delay
    do (i--);
    while (i != 0);
}

void wait_long(void)
{
  volatile unsigned int i;
   i = 50000;                              // Delay
    do (i--);
    while (i != 0);
}

void wait(unsigned int x)
{
  volatile unsigned int i=0;
    for(i;i<x;i++)
    {}
}
