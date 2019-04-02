#define QUEUE_CAPACITY 10

char buffer[QUEUE_CAPACITY];
unsigned int tail;
unsigned int head;

void ClearArray(char* array);
int InitQueue();
int QueueWriteData(char data);
int QueueReadData(char* data);