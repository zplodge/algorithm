/*
 * @Author: Leon
 * @emial: zhangpeng@tuya.com
 * @Date: 2019-10-16 23:33:08
 * @LastEditors: Leon
 * @LastEditTime: 2019-12-08 14:33:01
 * @file name: queue.c
 * @Description: this file is the queue source files 
 * @Copyright: 
 * @Company: 
 */

#include <stdio.h>
#include <string.h>
#include "queue.h"

queue_list_t *head = NULL, *tail = NULL;

/**
 * @description: init the queue
 * @param {type} NULL
 * @return: NULL
 */
void queue_init(void)
{
	head = (queue_list_t*)malloc(sizeof(queue_list_t));

	if(head)
	{
		memset(head, 0, sizeof(queue_list_t));
		tail = head;
	}
	else
	{
		printf("no enough space \n\r");
	}
}

/**
 * @description: push a item to queue
 * @param {input_data} queue item data
 * @return: TRUE or FALSE
 */
bool_t queue_push(queue_list_t* input_data)
{
	queue_list_t *push_postion = NULL;

	input_data->next = NULL;

	if(push_postion = (queue_list_t*)malloc(sizeof(queue_list_t)))
	{
		memset(push_postion, 0, sizeof(queue_list_t));
		memcpy(push_postion, input_data, sizeof(queue_list_t));
		
		tail->next = push_postion;
		tail = tail->next;
	}
	else
	{
		printf("no enough space \n\r");
		return FALSE;
	}
	
	return TRUE;
}

/**
 * @description: pop a item to queue
 * @param {out_data} queue item data
 * @return: TRUE or FALSE
 */
bool_t queue_pop(queue_list_t* out_data)
{
	queue_list_t *free_index = NULL;
	out_data->next = NULL;
	
	if(head == NULL || head == tail)
	{
		printf("queue empty");
		out_data = NULL;
		return FALSE;
	}

	free_index = head;
	head = head->next;
	
	if(head != NULL)
	{
		memcpy(out_data, head, sizeof(queue_list_t));
	}
	
	free(free_index);
	
	return TRUE;
}

void queue_deinit(void)
{
	queue_list_t *head_position = NULL;

	while(head != tail)
	{
		head_position = head;
		
		if(head->next)
		{
			head = head->next;
		}
		
		free(head_position);
	}
}
