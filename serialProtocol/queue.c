#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "queue.h"

queue_t serialDataSendQueue;

void queueInit(void)
{
    memset(&serialDataSendQueue, 0, sizeof(queue_t));
}

bool queuePush(uint8_t* data, uint8_t data_len, uint16_t seqNum)
{
    if(serialDataSendQueue.tail >= MAX_QUEUE_ITEM)
    {
        return false;
    }
    else if(serialDataSendQueue.currentPushOffset + data_len > MAX_PAYLOAD_SIZE)
    {
        return false;
    }
    
    if(serialDataSendQueue.tail == 0) /*the  queue is empty*/
    {
        serialDataSendQueue.seqNum[0] = seqNum;
        serialDataSendQueue.dataLen[0] = data_len;
        memcpy(serialDataSendQueue.data, data, data_len);
        serialDataSendQueue.currentPushOffset = data_len;
        serialDataSendQueue.totalItems++;
        serialDataSendQueue.tail++;
    }
    else
    {
        uint8_t i=0, index = 0;

        for(i=0; i<serialDataSendQueue.tail-1; i++)
        {
            if(serialDataSendQueue.seqNum[i] == seqNum)
            {
                return false; /*the data is in the queue already*/
            }
        }

        index = serialDataSendQueue.tail;
        serialDataSendQueue.seqNum[index] = seqNum;
        serialDataSendQueue.dataLen[index] = data_len;
        memcpy(&serialDataSendQueue.data[serialDataSendQueue.currentPushOffset], data, data_len);
        serialDataSendQueue.currentPushOffset += data_len;
        serialDataSendQueue.totalItems++;
        serialDataSendQueue.tail++;
    }
    return true;
}

queue_pop_t* queuePop(void)
{
    queue_pop_t popData;
    
    if(serialDataSendQueue.tail == 0) /*queue is empty*/
    {
        return null;
    }
    else if(serialDataSendQueue.totalItems == 0)/*queue is no items*/
    {
        serialDataSendQueue.head = 0;
        serialDataSendQueue.tail = 0;
        return null;
    }

    memset(&popData, 0, sizeof(queue_pop_t));
    if(serialDataSendQueue.head < serialDataSendQueue.tail)
    {
        uint8_t index = 0;

        index = serialDataSendQueue.head;
        popData.seqNum = serialDataSendQueue.seqNum[index];
        popData.dataLen = serialDataSendQueue.dataLen[index];
        memcpy(popData.data, serialDataSendQueue.data[serialDataSendQueue.currentPopOffset], popData.dataLen);
        serialDataSendQueue.currentPopOffset += popData.dataLen;
        serialDataSendQueue.head++;
        if((serialDataSendQueue.currentPopOffset == serialDataSendQueue.currentPushOffset) \
          || (serialDataSendQueue.head == serialDataSendQueue.tail))
          {
          /*all data has been pop from the queue, clear the queue*/
              queueInit();
          }
    }
    else
    {
        queueInit();
    }
}


