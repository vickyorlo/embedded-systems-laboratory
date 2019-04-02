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
#define ONEMS 0x0000186A
#define DELAY 1000
//delay is in miliseconds

unsigned int SpuriousInterruptCount = 0;

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

void ClearPITCValue()
{
  *AT91C_PITC_PIVR;
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

void DisplayFullNumberOnSegmentedDisplay(int milisecondCount)
{
 static int valueToDisplay = 0;
 static int tensDigit = 0;
 static int unitsDigit = 0;
 
 if (milisecondCount%2==0)
 {
	tensDigit = valueToDisplay/10;
	DisableRightDigit();
	DisplayDigitOnSegmentedDisplay(tensDigit);
	EnableLeftDigit();
 }
 else
 {
	unitsDigit = valueToDisplay%10;
	DisableLeftDigit();
	DisplayDigitOnSegmentedDisplay(unitsDigit);
	EnableRightDigit();
 }
 
  if (milisecondCount%DELAY==0)
 {
   valueToDisplay++;
   valueToDisplay = valueToDisplay%100;
 }
}

void PITCIRQHandler()
{
 static unsigned int milisecondCount=0;
 if ((*AT91C_PITC_PIMR && AT91C_PITC_PITIEN)	&
     (*AT91C_AIC_ISR == AT91C_ID_SYS)		&
     (*AT91C_PITC_PISR & AT91C_PITC_PITS)	)
 {
    ClearPITCValue();
    DisplayFullNumberOnSegmentedDisplay(milisecondCount);
    milisecondCount=(milisecondCount+1)%DELAY;
 }
 else SpuriousInterruptCount++;
}

void InitializePITC()
{
 SetPITCPeriodicIntervalValue(ONEMS); //function also disables timer and interrupts,
				      //make sure to enable them as needed
}

void InitializeSegmentDisplay(void)
{
  *AT91C_PIOB_PER = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|LEFTDIGIT|RIGHTDIGIT|BRIGHTNESS|SEGDOT; //turn on all related pins
  *AT91C_PIOB_OER = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|LEFTDIGIT|RIGHTDIGIT|BRIGHTNESS|SEGDOT; //set all related pins to output mode
  *AT91C_PIOB_CODR = SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG|SEGDOT|BRIGHTNESS; //turn off all segments and set brightness to full
  DisableRightDigit();
  DisableLeftDigit();
}

void InitializeAICForPITC()
{
  DisablePITCInterrupts();
  *AT91C_AIC_IDCR = 1 << AT91C_ID_SYS; 
  *AT91C_AIC_ICCR = 1 << AT91C_ID_SYS;
  
  AT91C_AIC_SVR[AT91C_ID_SYS] = (unsigned int) PITCIRQHandler; 
  AT91C_AIC_SMR[AT91C_ID_SYS] |= AT91C_AIC_SRCTYPE_EXT_POSITIVE_EDGE;
  AT91C_AIC_SMR[AT91C_ID_SYS] |=  AT91C_AIC_PRIOR_HIGHEST;
  
  EnablePITCInterrupts();
  *AT91C_AIC_IECR = 1 << AT91C_ID_SYS; 
}

int main(void)
{
  InitializePITC();
  InitializeSegmentDisplay();
  InitializeAICForPITC();
  EnablePITC();
  while(1)
  {
   
  }
  
}
