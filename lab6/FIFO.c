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
	int nextHead=((head+1)%QUEUE_CAPACITY);
  	if(nextHead==tail) //normal full condition
	{
	  return 0;
	}
	buffer[head]=data;
	head = nextHead;
	if(((head+1)%QUEUE_CAPACITY)==tail) //full after writing, allows for
	{				   //every written character to be displayed
	  return 0;
	} 
	else return 1;
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