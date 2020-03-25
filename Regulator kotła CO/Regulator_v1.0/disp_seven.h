//ledy sygnalizacji
#define led_pompa   BIT4  //ZIELONY
#define led_klapa   BIT3  //ZOLTY 
#define led_alarm   BIT2  //CZERWONY

//switche up, down
#define sw_up     BIT2
#define sw_down   BIT3



//******************DISPLAY
#define BIT_ALL (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7)

//digits
#define s_1 ~(b+c)
#define s_2 ~(a+b+g+e+d)
#define s_3 ~(a+b+c+d+g)
#define s_4 ~(b+c+g+f)
#define s_5 ~(a+f+g+c+d)
#define s_6 ~(a+f+g+e+d+c)
#define s_7 ~(a+b+c)
#define s_8 ~(a+b+c+d+e+f+g)
#define s_9 ~(a+b+c+d+f+g)
#define s_0 ~(a+b+c+d+e+f)
#define s_none ~(g)
#define s1_dp ~(dp)
#define none BIT_ALL


//bity 
#define com1 ~BIT6
#define com2 ~BIT7
#define a BIT0
#define b BIT1
#define c BIT2
#define d BIT3
#define e BIT4
#define f BIT5
#define g BIT6
#define dp BIT7


//port segmentu LCD
#define DIR_seg P3DIR
#define OUT_seg P3OUT

//port tranzsytory com
#define DIR_com P1DIR
#define OUT_com P1OUT

//port ledy sygnalizacji
#define DIR_led P2DIR
#define OUT_led P2OUT

//port switche 
#define DIR_SW P1DIR
#define sw_up BIT2
#define sw_down BIT3
#define PIN_sw P1IN



//zakresy temp
#define t0 415
#define t5 405
#define t10 394
#define t15 377
#define t20 374
#define t25 364
#define t30 355
#define t35 346
#define t40 337
#define t45 328
#define t50 320
#define t55 312
#define t60 304
#define t65 296
#define t70 289
#define t75 281
#define t80 275
#define t85 268
#define t90 261
#define t95 255
#define t100 249
#define t105 244
#define t110 237
#define t115 232
#define t120 227
