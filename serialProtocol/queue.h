#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef unsigned char  uint8_t
typedef unsigned int   uint16_t

#define MAX_PAYLOAD_SIZE 1000
#define MAX_POP_DATA_SIZE 128
#define MAX_QUEUE_ITEM   10
typedef struct
{
    uint8_t head, tail;
    uint8_t totalItems;
    uint8_t currentOffset;
    uint8_t seqNum[MAX_QUEUE_ITEM];
    uint8_t dataLen[MAX_QUEUE_ITEM];
    unit8_t data[MAX_PAYLOAD_SIZE];
}queue_t;

typedef struct
{
    uint8_t seqNum;
    uint8_t dataLen;
    uint8_t data[MAX_POP_DATA_SIZE];
}queue_pop_t;

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
    else if(serialDataSendQueue.currentOffset + data_len > MAX_PAYLOAD_SIZE)
    {
        return false;
    }
    
    if(serialDataSendQueue.tail == 0) /*the  queue is empty*/
    {
        serialDataSendQueue.seqNum[0] = seqNum;
        serialDataSendQueue.dataLen[0] = data_len;
        memcpy(serialDataSendQueue.data, data, data_len);
        serialDataSendQueue.currentOffset = data_len;
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
        memcpy(&serialDataSendQueue.data[serialDataSendQueue.currentOffset], data, data_len);
        serialDataSendQueue.currentOffset += data_len;
        serialDataSendQueue.totalItems++;
        serialDataSendQueue.tail++;
    }
    return true;
}

queue_pop_t* queuePop(void)
{
    queue_pop_t popData;
    
    if(serialDataSendQueue.tail == 0)
    {
        return null;
    }
    else if(serialDataSendQueue.totalItems == 0)
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
        memcpy(popData.data, serialDataSendQueue.data[serialDataSendQueue.]);
        
    }
}
#endif //__QUEUE_H__