#include <time.h>
void dbgu_print_ascii(const char *a) {}
#define SET_LCLICK (1 << 4)
#define SET_RCLICK (1 << 5)
#define SET_LEFT_LED (1 << 8)
#define SET_RIGHT_LED (1 << 29)
#define DELAY CLOCKS_PER_SEC*1000


int main(void)
{
    volatile unsigned int *PIOB_PER = (volatile unsigned int*) 0xFFFFF400; /*PER - PIO Enable Register*/
    volatile unsigned int *PIOC_PER = (volatile unsigned int*) 0xFFFFF600;
    
    volatile unsigned int *PIOB_OER = (volatile unsigned int*) 0xFFFFF410; /*OER - output enable register*/
    volatile unsigned int *PIOC_OER = (volatile unsigned int*) 0xFFFFF610;
    
    volatile unsigned int *PIOB_SODR = (volatile unsigned int*) 0xFFFFF430; /*SODR - Set Output Data Register*/
    volatile unsigned int *PIOC_SODR = (volatile unsigned int*) 0xFFFFF630;
    
    volatile unsigned int *PIOB_CODR = (volatile unsigned int*) 0xFFFFF434; /*CODR - Clear Output Data Register*/
    volatile unsigned int *PIOC_CODR = (volatile unsigned int*) 0xFFFFF634;
    
    volatile unsigned int *PIOC_ODR = (volatile unsigned int*) 0xFFFFF614; /*ODR - Output Disable Register*/
    volatile unsigned int *PIOC_PDSR = (volatile unsigned int*) 0xFFFFF63C; /*PDSR - Pin Data Status Register*/
    
    volatile unsigned int *PMC_PCER = (volatile unsigned int*) 0xFFFFFC10;
    
    
    *PMC_PCER = 1 << 4;
    
    *PIOB_PER = SET_LEFT_LED;
    *PIOC_PER = SET_RIGHT_LED;
    
    *PIOB_OER = SET_LEFT_LED;
    *PIOC_OER = SET_RIGHT_LED;
    
    *PIOC_SODR = SET_RIGHT_LED;
    
    *PIOB_CODR = SET_LEFT_LED;
    *PIOB_SODR = SET_LEFT_LED;    
    
    *PIOC_PER = SET_LCLICK;
    *PIOC_PER = SET_RCLICK;
    
    *PIOC_ODR = SET_LCLICK;
    *PIOC_ODR = SET_RCLICK;
    
    unsigned int counter = 0;
    short int on = 0;
    
    
    while(1)
    {
      counter+=1;
      if ((counter % (DELAY/5)) == 0)
      {
	if(!(*PIOC_PDSR & SET_RCLICK))
	  *PIOB_CODR = SET_LEFT_LED;
	if(!(*PIOC_PDSR & SET_LCLICK))
	  *PIOB_SODR = SET_LEFT_LED;
      }
      
      if(counter==DELAY)
      {
	if (on)
	{
	  *PIOC_SODR = SET_RIGHT_LED;
	  on = 0;
	}
	else
	{
	  *PIOC_CODR = SET_RIGHT_LED;
	  on=1;
	}
	counter=0;
      }
      
    }
}
