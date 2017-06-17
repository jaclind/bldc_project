/*__________________________________________________________________________________
  |                                                                                |
  |Header:       bldc_queue.h                                                      |
  |Version:      0.1                                                               |
  |Description   Circular FIFO queue for instructions and data.                    |                                                                               |
  |By:           Grupp 5                                                           |
  |                                                                                |
  |Last edit:    5/4 -17, 11:45                                                    |
  |________________________________________________________________________________|
                                                                                    */
#ifndef BLDC_FIFO_H_INCLUDED
#define BLDC_FIFO_H_INCLUDED
#define MAX_SIZE 64

typedef char eQueue;     //define queue to be of type char

typedef struct{
    eQueue instrQueue[MAX_SIZE];    // array for instructions
    eQueue dataQueue[MAX_SIZE];     // array for data
    int head;                       // front of queue
    int tail;                       // back of queue
    int maxSize;                    // maximum queue length
}sQueue;

void iniQueue(sQueue *pQueue, int maxSize);                             // initialize queue-struct
int enQueue(sQueue *pQueue, char instIn, char dataIn);                  // add instruction and data to corresponding queue, checks first if full. returns 0 if full, 1 if success.
int deQueue(sQueue *pQueue);                                            // remove first item in queue. checks first if empty. returns 0 if empty, 1 if success
int getQueueHead(sQueue *pQueue, char *pInstOut, char *pDataOut);       // get first item in queue. checks first if empty. returns 0 if empty, 1 if success
void clrQueue(sQueue *pQueue);                                          // clear queue
int getQueueSize(sQueue *pQueue);                                       // returns size of queue
int isFull(sQueue *pQueue);                                             // check if queue is full, 1 = full
int isEmpty(sQueue *pQueue);                                            // check if queue is empty, 1 = empty
void displayQueue(sQueue *pQueue);                                      // mainly for testing, prints all items in both queues.

#endif // BLDC_FIFO_H_INCLUDED
