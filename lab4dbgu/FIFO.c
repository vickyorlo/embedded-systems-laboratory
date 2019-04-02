#include "FIFO.h"

void ClearArray(char* array)
{
  int index;
  for (index=0;index<QUEUE_CAPACITY;index++)
  {
    array[index]=0;
  }
}

int InitQueue()
{
	tail = head = 0;
	ClearArray(buffer);
	return 0;
}

int QueueWriteData(char data)
{
	if(((head+1)%QUEUE_CAPACITY)==tail)
	{
	  buffer[head]='\0';
	  return 0;
	}
	buffer[head]=data;
	head = (head+1)%QUEUE_CAPACITY;
	return 1;
}

int QueueReadData(char* data)
{
	if(head==tail)
	{
	  return 0;
	}
	*data = buffer[tail];
	tail = (tail+1)% QUEUE_CAPACITY;
	return 1;
}


int QueueReadString(char* string)
{
  do
  {
    QueueReadData(string);
  } while (*string++);	
  return 0;	
}
