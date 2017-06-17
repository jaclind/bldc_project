/*__________________________________________________________________________________
  |                                                                                |
  |Header:       bldc_queue.c                                                      |
  |Version:      0.1                                                               |
  |Description   Circular FIFO queue for instructions and data.                    |
  |By:           Grupp 5                                                           |
  |                                                                                |
  |Last edit:    5/4 -17, 11:45                                                    |
  |________________________________________________________________________________|
                                                                                    */
#include <stdio.h>
#include "bldc_queue.h"
//#define MAX_SIZE 64

void iniQueue(sQueue *pQueue, int maxSize)
{
    pQueue->maxSize = maxSize;
    pQueue->head = 0;
    pQueue->tail = 0;
}

int enQueue(sQueue *pQueue, char instIn, char dataIn)
{
    if(isFull(pQueue))
    {
        return 0;
    }
    else
    {
        pQueue->instrQueue[pQueue->tail] = instIn;
        pQueue->dataQueue[pQueue->tail] = dataIn;
        if(pQueue->tail < pQueue->maxSize)     // roll-over
            pQueue->tail++;
        else
            pQueue->tail = 0;
        return 1;
    }
}

int getQueueHead(sQueue *pQueue, char *pInstOut, char *pDataOut)
{
    if(isEmpty(pQueue))
    {
        return 0;
    }
    else
    {
        *pInstOut = pQueue->instrQueue[pQueue->head];
        *pDataOut = pQueue->dataQueue[pQueue->head];
        return 1;
    }
}

int deQueue(sQueue *pQueue)
{
    if(isEmpty(pQueue))
    {
        return 0;
    }
    else
    {
        if(pQueue->head < pQueue->maxSize)
            pQueue->head++;
        else
            pQueue->head = 0;
    return 1;
    }
}

void clrQueue(sQueue *pQueue)
{
    pQueue->head = 0;
    pQueue->tail = 0;
}

int getQueueSize(sQueue *pQueue)        // if empty return 0, else return size
{
    if(isEmpty(pQueue))
        return 0;
    else
    {
        if(pQueue->head < pQueue->tail)
            return (pQueue->tail - pQueue->head);
        else
            return ((pQueue->maxSize - pQueue->head) + pQueue->tail)+1;
    }
}

int isEmpty(sQueue *pQueue)
{
    if(pQueue->head == pQueue->tail)
        return 1;
    else
        return 0;
}

int isFull(sQueue *pQueue)
{
    if((pQueue->head == pQueue->tail+1) || ((pQueue->tail == pQueue->maxSize) && (pQueue->head == 0)))
        return 1;
    else
        return 0;
}

/*
void displayQueue(sQueue *pQueue)
{
if(isEmpty(pQueue))
{
    printf("Queue EMPTY\n");
}else{
    if(pQueue->head < pQueue->tail)
    {
        for(int i = pQueue->head; i < pQueue->tail; i++)
            {
                printf("%d",pQueue->instrQueue[i]);
                printf("     %d\n",pQueue->dataQueue[i]);
            }
    }
    else
    {
        for(int i = pQueue->head; i <= pQueue->maxSize; i++)
        {
            printf("%d",pQueue->instrQueue[i]);
            printf("     %d\n",pQueue->dataQueue[i]);
        }
        for(int i = 0; i <= pQueue->tail-1; i++)
        {
            printf("%d",pQueue->instrQueue[i]);
            printf("     %d\n",pQueue->dataQueue[i]);
        }
    }
    printf("END WRITE\n");
}
}*/
