#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef unsigned char  uint8_t
typedef unsigned int   uint16_t


#define MAX_PAYLOAD_SIZE  1000
#define MAX_POP_DATA_SIZE 128
#define MAX_QUEUE_ITEM    10

typedef struct
{
    uint8_t seqNum;
    uint8_t dataLen;
    uint8_t data[MAX_POP_DATA_SIZE];
}queue_pop_t;


typedef struct
{
    uint8_t head, tail;
    uint8_t totalItems;
    uint8_t currentPopOffset;
    uint8_t currentPushOffset;
    uint8_t seqNum[MAX_QUEUE_ITEM];
    uint8_t dataLen[MAX_QUEUE_ITEM];
    unit8_t data[MAX_PAYLOAD_SIZE];
}queue_t;

#endif //__QUEUE_H__