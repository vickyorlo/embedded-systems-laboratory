#include<stdio.h>
#include<stdlib.h>
#include "AT91SAM9263.h"
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
#define DELAY 1000
#define SINGLEDIGITDELAY 5
  
void DisplayDigitOnSegmentedDisplay(int digit)
{
  switch (digit)
  {
    case 0:
      *AT91C_PIOB_SODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF;
      *AT91C_PIOB_CODR = SEGG;
      break;
    case 1:
      *AT91C_PIOB_SODR = SEGB|SEGC;
      *AT91C_PIOB_CODR = SEGA|SEGD|SEGE|SEGF|SEGG;
      break;
    case 2:
      *AT91C_PIOB_SODR = SEGA|SEGB|SEGG|SEGE|SEGD;
      *AT91C_PIOB_CODR = SEGF|SEGC;
      break;
    case 3:
      *AT91C_PIOB_SODR = SEGA|SEGB|SEGC|SEGD|SEGG;
      *AT91C_PIOB_CODR = SEGE|SEGF;
      break;
    case 4:
      *AT91C_PIOB_SODR = SEGF|SEGG|SEGB|SEGC;
      *AT91C_PIOB_CODR = SEGA|SEGE|SEGD;
      break;
    case 5:
      *AT91C_PIOB_SODR = SEGA|SEGF|SEGG|SEGC|SEGD;
      *AT91C_PIOB_CODR = SEGB|SEGE;
      break;
    case 6:
      *AT91C_PIOB_SODR = SEGA|SEGC|SEGG|SEGE|SEGD|SEGF;
      *AT91C_PIOB_CODR = SEGB;
      break;
    case 7:
      *AT91C_PIOB_SODR = SEGA|SEGB|SEGC;
      *AT91C_PIOB_CODR = SEGE|SEGF|SEGG|SEGD;
      break;
    case 8:
      *AT91C_PIOB_SODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG;
      break;
    case 9:
      *AT91C_PIOB_SODR = SEGA|SEGB|SEGG|SEGF|SEGD|SEGC;
      *AT91C_PIOB_CODR = SEGE;
      break;
  }
}

void EnablePITC()
{
  *AT91C_PITC_PIMR |= AT91C_PITC_PITEN;
}

void DisablePITC()
{
  *AT91C_PITC_PIMR &= ~AT91C_PITC_PITEN;
}

void DisablePITCInterrupts()
{
  *AT91C_PITC_PIMR &= ~AT91C_PITC_PITIEN;
}

void EnablePITCInterrupts()
{
  *AT91C_PITC_PIMR |= AT91C_PITC_PITIEN;  
}

void SetPITCPeriodicIntervalValue(unsigned int piv)
{
 *AT91C_PITC_PIMR = (AT91C_PITC_PIV & piv);
}

void InitializePITC()
{
 SetPITCPeriodicIntervalValue(0x0000186A); /*0.001 s*/
 DisablePITCInterrupts();
 DisablePITC();
}

void ClearPITCValue()
{
  *AT91C_PITC_PIVR;
}

void delay_ms(unsigned int delayInMiliseconds)
{
 ClearPITCValue();
 EnablePITC();
 unsigned int milisecondsPassed;

 for (milisecondsPassed = 0; milisecondsPassed <= delayInMiliseconds; milisecondsPassed++)
 {
   while((*AT91C_PITC_PISR & AT91C_PITC_PITS) == 0);
   ClearPITCValue();
 }
 DisablePITC();
}

void EnableRightDigit()
{
	*AT91C_PIOB_CODR=RIGHTDIGIT;
}

void DisableRightDigit()
{
	*AT91C_PIOB_SODR=RIGHTDIGIT;
}

void EnableLeftDigit()
{
	*AT91C_PIOB_CODR=LEFTDIGIT;
}

void DisableLeftDigit()
{
	*AT91C_PIOB_SODR=LEFTDIGIT;
}

void DisplayFullNumberOnSegmentedDisplay(int valueToDisplay, int timeToDisplayFor)
{
 valueToDisplay = valueToDisplay%100; //overflow protection
 int tensDigit = valueToDisplay/10;
 int unitsDigit = valueToDisplay%10;
 
 unsigned int timer;
 
    for (timer=0;timer<=timeToDisplayFor/(SINGLEDIGITDELAY*2);timer++)
    {
	DisableRightDigit();
	DisplayDigitOnSegmentedDisplay(tensDigit);
	EnableLeftDigit();
	delay_ms(SINGLEDIGITDELAY);
	DisableLeftDigit();
	DisplayDigitOnSegmentedDisplay(unitsDigit);
	EnableRightDigit();
	delay_ms(SINGLEDIGITDELAY);
    }
    
}

void InitializeSegmentDisplay(void)
{
  *AT91C_PIOB_PER = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|LEFTDIGIT|RIGHTDIGIT|BRIGHTNESS|SEGDOT; //turn on all related pins
  *AT91C_PIOB_OER = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|LEFTDIGIT|RIGHTDIGIT|BRIGHTNESS|SEGDOT; //set all related pins to output mode
  *AT91C_PIOB_CODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|SEGDOT|BRIGHTNESS; //turn off all segments and set brightness to full
  DisableRightDigit();
  DisableLeftDigit();
}

int main(void)
{
  InitializePITC();
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
