#include<stdio.h>
#include<stdlib.h>
void dbgu_print_ascii(const char *a) {}

#define SEGA (1<<25)
#define SEGB (1<<24)
#define SEGC (1<<22)
#define SEGD (1<<21)
#define SEGE (1<<20)
#define SEGF (1<<27)
#define SEGG (1<<26)
#define SEGDOT (1<<23)
#define LEFTDIGIT (1<<28)
#define RIGHTDIGIT (1<<30)
#define BRIGHTNESS (1<<29)
#define DELAY 10000
#define DELAYPROPORTION 100

  volatile unsigned int* PIOB_PER = (volatile unsigned int*) 0xFFFFF400;
  volatile unsigned int* PIOB_OER = (volatile unsigned int*) 0xFFFFF410;
  volatile unsigned int* PIOB_ODR = (volatile unsigned int*) 0xFFFFF414;
  volatile unsigned int* PIOB_SODR = (volatile unsigned int*) 0xFFFFF430;
  volatile unsigned int* PIOB_CODR = (volatile unsigned int*) 0xFFFFF434;
  
void DisplayDigitOnSegmentedDisplay(int digit)
{
  switch (digit)
  {
    case 0:
      *PIOB_SODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF;
      *PIOB_CODR = SEGG;
      break;
    case 1:
      *PIOB_SODR = SEGB|SEGC;
      *PIOB_CODR = SEGA|SEGD|SEGE|SEGF|SEGG;
      break;
    case 2:
      *PIOB_SODR = SEGA|SEGB|SEGG|SEGE|SEGD;
      *PIOB_CODR = SEGF|SEGC;
      break;
    case 3:
      *PIOB_SODR = SEGA|SEGB|SEGC|SEGD|SEGG;
      *PIOB_CODR = SEGE|SEGF;
      break;
    case 4:
      *PIOB_SODR = SEGF|SEGG|SEGB|SEGC;
      *PIOB_CODR = SEGA|SEGE|SEGD;
      break;
    case 5:
      *PIOB_SODR = SEGA|SEGF|SEGG|SEGC|SEGD;
      *PIOB_CODR = SEGB|SEGE;
      break;
    case 6:
      *PIOB_SODR = SEGA|SEGC|SEGG|SEGE|SEGD|SEGF;
      *PIOB_CODR = SEGB;
      break;
    case 7:
      *PIOB_SODR = SEGA|SEGB|SEGC;
      *PIOB_CODR = SEGE|SEGF|SEGG|SEGD;
      break;
    case 8:
      *PIOB_SODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG;
      break;
    case 9:
      *PIOB_SODR = SEGA|SEGB|SEGG|SEGF|SEGD|SEGC;
      *PIOB_CODR = SEGE;
      break;
  }
}

void WaitForDisplay(int time)
{
  volatile int i;
  for(i=0;i<=time;i++);
}

void EnableRightDigit()
{
	*PIOB_CODR=RIGHTDIGIT;
}

void DisableRightDigit()
{
	*PIOB_SODR=RIGHTDIGIT;
}

void EnableLeftDigit()
{
	*PIOB_CODR=LEFTDIGIT;
}

void DisableLeftDigit()
{
	*PIOB_SODR=LEFTDIGIT;
}

void DisplayFullNumberOnSegmentedDisplay(int valueToDisplay, int timeToDisplayFor)
{
 valueToDisplay = valueToDisplay%100; //overflow protection
 int tensDigit = valueToDisplay/10;
 int unitsDigit = valueToDisplay%10;
 
 unsigned int timer;
 
    for (timer=0;timer<=timeToDisplayFor;timer++)
    {
	DisableRightDigit();
	DisplayDigitOnSegmentedDisplay(tensDigit);
	EnableLeftDigit();
	WaitForDisplay(DELAY/DELAYPROPORTION);
	DisableLeftDigit();
	DisplayDigitOnSegmentedDisplay(unitsDigit);
	EnableRightDigit();
	WaitForDisplay(DELAY/DELAYPROPORTION);
    }
    
}

void InitializeSegmentDisplay(void)
{
  *PIOB_PER = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|LEFTDIGIT|RIGHTDIGIT|BRIGHTNESS|SEGDOT; //turn on all related pins
  *PIOB_OER = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|LEFTDIGIT|RIGHTDIGIT|BRIGHTNESS|SEGDOT; //set all related pins to output mode
  *PIOB_CODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|SEGDOT|BRIGHTNESS; //turn off all segments and set brightness to full
  DisableRightDigit();
  DisableLeftDigit();
}

int main(void)
{
  
  InitializeSegmentDisplay();
  int currentCount = 0;
  while(1)
  {
    DisplayFullNumberOnSegmentedDisplay(currentCount,DELAY);
    currentCount++;
    if (currentCount>=100)
    {
      currentCount=0;
    }
  }
  
}
