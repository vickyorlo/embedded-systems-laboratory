#include"AT91SAM9263.h"
#include"DBGU.h"
#include"FIFO.h"
#define ASCIICAPITALOFFSET 32


void dbgu_print_ascii(const char *a) {
 while(!(*AT91C_DBGU_CSR & AT91C_US_TXRDY)){}
 *AT91C_DBGU_THR = *a;
}

char SwitchLetterCase(char letter)
{
  if(letter >= 'a' && letter <= 'z')
  letter -= ASCIICAPITALOFFSET;
  else if (letter >= 'A' && letter <= 'Z')
  letter += ASCIICAPITALOFFSET;
  return letter;
}

void SwitchStringCase(char* buffer){
  while(*buffer){
    DBGU_SendData(SwitchLetterCase(*buffer));
    buffer++;
  }
}

void PrintAlphabet(){
  int letter = 'a';
  while(letter <= 'z'){
    DBGU_SendData(letter);
    letter++;
  }
} 

void SwitchEchoCase(){
  char letter= ' ';
  DBGU_ReadData(&letter);
  letter = SwitchLetterCase(letter);
  DBGU_SendData(letter);  
}

int main(void)
{
 DBGU_Init();
 InitQueue();
 DBGU_PrintString("\raeiouabcd\n");
 DBGU_PrintString("snake\n");
 SwitchStringCase("this is lowercase\r\n");
 //PrintAlphabet();
 char readBuffer[QUEUE_CAPACITY];
 while(1)
 {
   //SwitchEchoCase();
   DBGU_ReadStringToFifo();
   DBGU_PrintString("\r\nread data\r\n");
   QueueReadString(readBuffer);
   DBGU_PrintString(readBuffer);
 }
 return 0;
}
