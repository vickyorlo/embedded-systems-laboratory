#include "AT91SAM9263.h"
#define MASTERCLOCK 100000000
#define BAUDRATE 115200

int DBGU_Init(void);
int DBGU_SendData(char data);
int DBGU_ReadData(char* data);
void DBGU_PrintString(char* buffer);
void DBGU_ReadStringToFifo();