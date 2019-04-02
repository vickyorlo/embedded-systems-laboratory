#include "DBGU.h"

int DBGU_Init(void)
{

 //disable rx and tx
 *AT91C_DBGU_CR  = AT91C_US_RXDIS;
 *AT91C_DBGU_CR  = AT91C_US_TXDIS;
 //reset receiver and transmitter
 *AT91C_DBGU_CR  |= AT91C_US_RSTRX;
 *AT91C_DBGU_CR  |= AT91C_US_RSTTX;
 //enable and configure i/o
 *AT91C_PIOC_PDR = AT91C_PC30_DRXD;
 *AT91C_PIOC_PDR = AT91C_PC31_DTXD;
 *AT91C_PIOC_ASR = AT91C_PC30_DRXD;
 *AT91C_PIOC_ASR = AT91C_PC31_DTXD;
 //reset status registers
 *AT91C_DBGU_CR  |= AT91C_US_RSTSTA;
 //enable no parity bit mode
 *AT91C_DBGU_MR  |= AT91C_US_PAR_NONE;
 *AT91C_DBGU_MR  |= AT91C_US_CHMODE_NORMAL;
 //disable all interrupts
 *AT91C_DBGU_IDR = 0xFFFFFFFF;
 //set baudrate
 *AT91C_DBGU_BRGR = (unsigned int)(MASTERCLOCK/(16*BAUDRATE));
 //enable rx and tx
 *AT91C_DBGU_CR  = AT91C_US_RXEN;
 *AT91C_DBGU_CR  = AT91C_US_TXEN;
 return 0;
}

int DBGU_SendData(char data)
{
 while(!(*AT91C_DBGU_CSR & AT91C_US_TXRDY)){}
 *AT91C_DBGU_THR = data;
 return 0;
}

int DBGU_ReadData(char* data)
{
 while(!(*AT91C_DBGU_CSR & AT91C_US_RXRDY));
 *data = *AT91C_DBGU_RHR;
 return 0;
}

void DBGU_PrintString(char* buffer){
  while(*buffer){
    DBGU_SendData(*buffer++);
  }  
}