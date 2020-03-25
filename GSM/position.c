#include  <msp430x44x.h>
//Za wzór segmentow jest brany segment 8-6 , reszta to przesuwanie i kombinacja
#define a BIT0
#define b BIT1
#define c BIT2
#define d BIT3
#define i BIT4
#define j BIT5
#define k BIT6
#define l BIT7
#define h BIT0
#define g BIT1
#define f BIT2
#define e BIT3
#define p BIT4
#define o BIT5
#define m BIT6
#define n BIT7




// **********************************************  cleardipsp ******************************************************** 
void clean_disp(void)
{
  unsigned char x;
    for (x=0; x<19; x++)
    {
      LCDMEM[x] = CLEAN;  
    }
}






// **********************************************  cleardipsp ******************************************************** 
void wait(void)
{ unsigned int zmienna_x;
   for (zmienna_x = 0; zmienna_x < 10000; zmienna_x++); 
   { 
     int zmienna_y;
      for (zmienna_y = 0; zmienna_y < 10000; zmienna_y++);  
      {

      }
   } 
}
void wait2(void)
{ unsigned int zmienna_x;
   for (zmienna_x = 0; zmienna_x < 10; zmienna_x++); 
   { 
     wait();
   } 
}
// **********************************************  write ******************************************************** 
void write(int position,char letter) // writes a single character on the LCD. User can specify position as well
{
  unsigned int seg_abcd,seg_hgfe,seg_ijkl;
  unsigned int move_s1,move_s2,move_s3;
  if(position==8 || position==7 || position==6)
  {
    seg_abcd=(8-position);
    seg_hgfe=(8+position);  //segmenty2....8
    move_s1=0;    //przesuniecie seg.abcd
    move_s2=0;  //przesuniecie seg.hgfe
  }
  else if(position==5||position==4||position==3||position==2)
  {
    seg_abcd=(8-position);
    seg_ijkl=(9-position);
    seg_hgfe=(8+position);  //segmenty2....8  
    move_s1=4;
    move_s2=0;  
    move_s3=4; 
  }
 else if(position==1)
  {
    seg_abcd=(8-position);
    seg_ijkl=(9-position);
    seg_hgfe=(9-position); 
    move_s1=4;
    move_s2=4; 
    move_s3=4;          //poprawienie niektorych wskazan na wyswietlaczu
    switch(letter){ case '2':  LCDMEM[seg_ijkl+1]=BIT1;break;       case '3': LCDMEM[seg_ijkl+1]=BIT1; break;
                    case '4':  LCDMEM[seg_ijkl+1]=BIT1;break;       case '5': LCDMEM[seg_ijkl+1]=BIT1; break;
                    case '6':  LCDMEM[seg_ijkl+1]=BIT1;break;       case '8': LCDMEM[seg_ijkl+1]=BIT1; break;
                    case '9':  LCDMEM[seg_ijkl+1]=BIT1;break;       
                   
                    case 'A':  LCDMEM[seg_ijkl+1]=BIT1;break;       case 'R': LCDMEM[seg_ijkl+1]=BIT1; break;
                    case 'B':  LCDMEM[seg_ijkl+1]=BIT1;break;       case 'P': LCDMEM[seg_ijkl+1]=BIT1; break;
                    case 'E':  LCDMEM[seg_ijkl+1]=BIT1;break;       case 'S': LCDMEM[seg_ijkl+1]=BIT1; break; 
                    case 'F':  LCDMEM[seg_ijkl+1]=BIT1;break;       case 'K': LCDMEM[seg_ijkl+1]=BIT1; break;
                    case 'H':  LCDMEM[seg_ijkl+1]=BIT1;break;       case 'I':  LCDMEM[seg_ijkl+1]=BIT3;break; 
                    case 'J':  LCDMEM[seg_ijkl+1]=BIT3;break;       case 'T':  LCDMEM[seg_ijkl+1]=BIT3;break;
                    case 'M':  LCDMEM[seg_ijkl+1]=BIT0;break;       case 'N':  LCDMEM[seg_ijkl+1]=BIT0;break;
                    case 'W':  LCDMEM[seg_ijkl+1]=BIT2;break;       case 'Y':  LCDMEM[seg_ijkl+1]=BIT0|BIT3;break;  
                    case 'X':  LCDMEM[seg_ijkl+1]=BIT2|BIT0;break;  case 'Z':  LCDMEM[seg_ijkl+1]=BIT2;break;
 
                    case '+':  LCDMEM[seg_ijkl+1]=BIT1|BIT3;break;  case '>':  LCDMEM[seg_ijkl+1]=BIT0|BIT2;break;
                    case '-':  LCDMEM[seg_ijkl+1]=BIT1;break;       case ':':  LCDMEM[seg_ijkl+1]=BIT3;break;      
                    case '/':  LCDMEM[seg_ijkl+1]=BIT2;break;
    }        
    }
 
  //************************glowna petal swith
       switch(letter)
      {//SEGMENT                               pomn
      case '1':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c)<<move_s1));      break;
      case '2':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+d+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+f+e+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '3':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d+k)<<move_s1));   LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+h+o)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '4':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c+k)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((g+o)<<move_s2));         LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '5':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+c+d+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+g+e+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '6':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+c+d+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+g+f+e+o)<<move_s2));   LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3)); break;
      case '7':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h)<<move_s2));            break;
      case '8':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d+k)<<move_s1));   LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h+o)<<move_s2));   LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '9':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d+k)<<move_s1));   LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+g+e+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '0':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d)<<move_s1));   LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h)<<move_s2));   LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      
      case 'A':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+h+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'B':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d+k)<<move_s1));   LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h+o)<<move_s2));   LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'C':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+d)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h)<<move_s2));     ;break;
      case 'D':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d+k)<<move_s1));   LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h)<<move_s2));     ;break;
      case 'E':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+k+d)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h+o)<<move_s2));   LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'F':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+k)<<move_s1));         LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+h+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'G':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+c+d+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'H':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c+k)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+o)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'I':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((i)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((n)<<move_s2));           LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((i)>>move_s3));break;
      case 'J':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((i)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((n+e)<<move_s2));         LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((i)>>move_s3));break;
      case 'K':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((j+l)<<move_s1));         LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+o)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j+l)>>move_s3));break;
      case 'L':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((d)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+e)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      case 'M':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c+j)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((p+f+g)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j)>>move_s3));break;
      case 'N':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c+l)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+p)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((l)>>move_s3));break;
      case 'O':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g+h)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      case 'P':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+k)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+h+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'R':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+l+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+h+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k+l)>>move_s3));break;
      case 'S':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+c+d+k)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+g+e+o)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case 'T':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+i)<<move_s1));         LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+n)<<move_s2));         LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((i)>>move_s3));break;
      case 'U':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c+d)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e+f+g)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      case 'W':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((b+c+l)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+m)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((l)>>move_s3));break;
      case 'X':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((j+l)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((p+m)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j+l)>>move_s3));break;
      case 'Y':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((j)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((p+n)<<move_s2));         LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j)>>move_s3));break;
      case 'Z':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+j+d)<<move_s1));       LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((h+m+e)<<move_s2));       LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j)>>move_s3));break;
      case ':':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((i)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((n)<<move_s2));           LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((i)>>move_s3));break;
      case '>':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((0)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((p+m)<<move_s2));         LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      case '<':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((j+l)<<move_s1));         LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((0)<<move_s2));           LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j+l)>>move_s3));break;
      case '+':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((i+k)<<move_s1));         LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((n+o)<<move_s2));         LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((i+k)>>move_s3));break;
      case '-':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((k)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((o)<<move_s2));           LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((k)>>move_s3));break;
      case '/':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((j)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((m)<<move_s2));           LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((j)>>move_s3));break;
      case '[':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((0)<<move_s1));           LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((f+g+h+e)<<move_s2));     LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      case ']':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((a+b+c+d)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((0)<<move_s2));           LCDMEM[seg_ijkl]=(LCDMEM[seg_ijkl]|((0)>>move_s3));break;
      case '_':  LCDMEM[seg_abcd]=(LCDMEM[seg_abcd]|((d)<<move_s1));     LCDMEM[seg_hgfe]=(LCDMEM[seg_hgfe]|((e)<<move_s2));           ;break;}    
}

// **********************************************  zdanie ******************************************************** 
void write_sent(unsigned int start_poz , unsigned char *send_LCD , unsigned int ile)
{   
  unsigned int count_1;
   clean_disp();
  for(count_1=0;count_1<ile;count_1++)
  {
  write(start_poz+count_1,send_LCD[count_1]);
  }
}

// **********************************************  zdanie ******************************************************** 
void write_sent_one(unsigned int start_poz , unsigned char *send_LCD , unsigned int ile)
{   
  unsigned int count_1;
  for(count_1=0;count_1<ile;count_1++)
  {
  write(start_poz+count_1,send_LCD[count_1]);
  }
}
