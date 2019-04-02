#include "AT91SAM9263.h"
#include "DBGU.h"
#include <stdio.h>

#define ONE_MILISECOND 0x186A
#define SPI_CLOCK AT91C_PIO_PB14
#define SPI_CS AT91C_PIO_PB15
#define SPI_DATA AT91C_PIO_PB12
#define SPI_DATA_ID 12
#define DELAY_FOR_MEASUREMENT 350
#define TMP121_DATA_OFFSET 3
#define TMP121_DATA_RESOLUTION 0.0625
#define TMP121_DATA_LENGTH 16
#define READOUT_LENGTH 30

void dbgu_print_ascii(const char *a) {}

void InitSPI(void);
void InitializePITC(void);
void EnablePITC(void);
void DisablePITC(void);
void ClearPITCValue(void);
void MasterClockHigh();
void MasterClockLow();
void ChipSelectTMP121();
void ChipDeselectTMP121();
short unsigned int ReadSPIData();
float ReadTMP121Temperature();
float ConvertTMP121ToFloat(short int rawData);
int ReadTMP121Register(short int* data);

void PIT_Delay(unsigned int delayInMiliseconds)
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

int main(void)
{
    InitSPI();
    InitializePITC();
    DBGU_Init();
    DBGU_PrintString("\r\n\r\nThermometer Initialized\r\n\r\n");
    ReadTMP121Temperature(); //Empty reading, since first read is uncertain
    char TemperatureReadout[READOUT_LENGTH];
    while (1)
    {
	sprintf(TemperatureReadout,"\r\nRead Temperature: %f",ReadTMP121Temperature());
	DBGU_PrintString(TemperatureReadout);
    }
}


float ReadTMP121Temperature()
{
	short int ReadByte;
	PIT_Delay(DELAY_FOR_MEASUREMENT);
	ChipSelectTMP121();
	ReadTMP121Register(&ReadByte);
	ChipDeselectTMP121();
	return ConvertTMP121ToFloat(ReadByte);
}


int ReadTMP121Register(short int* data)
{
      int BitNumber;
	*data=0;
	for (BitNumber = (TMP121_DATA_LENGTH-1); BitNumber >= 0; BitNumber--)
	{
	    MasterClockLow();
	    *data |= (ReadSPIData() << BitNumber);
	    MasterClockHigh();
	}
	return 0;
}

float ConvertTMP121ToFloat(short int rawData)
{
 return (rawData >> TMP121_DATA_OFFSET) * TMP121_DATA_RESOLUTION;
}

void MasterClockHigh()
{
    *AT91C_PIOB_SODR = SPI_CLOCK;
}

void MasterClockLow()
{
    *AT91C_PIOB_CODR = SPI_CLOCK;
}

void ChipSelectTMP121()
{
    *AT91C_PIOB_CODR = SPI_CS;
}

void ChipDeselectTMP121()
{
    *AT91C_PIOB_SODR = SPI_CS;
}

short unsigned int ReadSPIData()
{
    return ((*AT91C_PIOB_PDSR) >> SPI_DATA_ID) & 1 ; 
}

void InitSPI(void)
{
    *AT91C_PIOB_IDR = 0xFFFFFFFF;
    *AT91C_PIOB_OER = SPI_CLOCK;
    *AT91C_PIOB_PER = SPI_DATA;
    *AT91C_PIOB_ODR = SPI_DATA;
    *AT91C_PIOB_PER = SPI_CS;
    *AT91C_PIOB_OER = SPI_CS;
    *AT91C_PMC_PCER = 1 << AT91C_ID_PIOB;
    *AT91C_PIOB_SODR = SPI_CLOCK;
    *AT91C_PIOB_SODR = SPI_CS;
}

void InitializePITC(void)
{    
    AT91C_BASE_PITC->PITC_PIMR = ONE_MILISECOND;
    AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITIEN;
}

void EnablePITC(void)
{
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN;
}

void DisablePITC(void)
{
    AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITEN;
}

void ClearPITCValue(void)
{
    AT91C_BASE_PITC->PITC_PIVR;
}