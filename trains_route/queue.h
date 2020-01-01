/*
 * @Author: Leon
 * @emial: zhangpeng@tuya.com
 * @Date: 2019-10-16 23:34:36
 * @LastEditors: Leon
 * @LastEditTime: 2019-12-08 14:37:39
 * @file name: queue.h
 * @Description: this file is the queue header files
 * @Copyright: 
 * @Company: 
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef  char bool_t;

#define FALSE  0
#define TRUE   1
#define MAX_QUEUE_SIZE  1000
#define MAX_INPUT_NODES  1001
#define INVALID_PATH  127
#define MAX_TEST_CASE_NODES 1000
#define INVALID_DISTANCE    0xFFFF

typedef enum
{
	ROUTE_DISTANCE = 1,
	MIN_DISTANCE,
	EQUAL_DISTANCE,
	SHORTEST_DISTANCE,
	NUM_WITH_MAX_DISTANCE,
}DIS_TYPE_E;


typedef struct queue_list
{
	struct queue_list* next;
	int node;
	int hops;
	int distance;
	bool_t flag;	
}queue_list_t;

extern queue_list_t *head, *tail;

extern void queue_init(void);
extern bool_t queue_push(queue_list_t* input_data);
extern bool_t queue_pop(queue_list_t* output_data);
extern void queue_deinit(void);

#endif