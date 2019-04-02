#include "AT91SAM9263.h"
#define MASTERCLOCK 100000000
#define BAUDRATE 115200

int DBGU_Init(void);
void EnableDBGUReceiver();
void EnableDBGUTransmitter();
void DisableDBGUTransmitter();
void DisableDBGUReceiver();

int DBGU_SendData(char data);
int DBGU_ReadData(char* data);
void InitializeAICForDBGU();

extern unsigned int TransferErrors;
extern unsigned int SpuriousInterrupts;