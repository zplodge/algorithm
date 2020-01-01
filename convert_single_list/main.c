/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @Date: 2020-01-01 20:10:41
 * @LastEditors  : Leon
 * @LastEditTime : 2020-01-01 21:10:40
 * @file name: 
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// #define DEBUG

int main(void)
{
	int test_case_num = 0;

	freopen("test_input.txt", "r", stdin);

	if(scanf("%d", &test_case_num) == EOF)
	{
		printf("no case \r\n");
		return 0;
	}

	queue_init();
	for(int i= 0; i< test_case_num; i++)
	{
		queue_list_t input_data;
		int node = 0;

		memset(&input_data, 0, sizeof(queue_list_t));
		if(scanf("%d", &node) == EOF)
		{
			printf("node data is error \r\n");
			return 0;
		}

		input_data.node = node;

		queue_push(&input_data);
	}
#ifdef DEBUG
	for(int i=0; i< test_case_num; i++)
	{
		if(head->next)
		{
			head = head->next;
		}
		printf("%d \n", head->node);
	}
#endif

	queue_list_t* current_node = NULL;
	queue_list_t* temp_node = NULL;
	queue_list_t* prev_node = NULL;
	if(head)
	{
		current_node = head;

		while(current_node)
		{
			temp_node = current_node;
			current_node = current_node->next;
			temp_node->next = prev_node;
			prev_node = temp_node;

		}
	}

	for(int i=0; i< test_case_num; i++)
	{
		printf("%d \n", temp_node->node);
		if(temp_node->next)
		{
			temp_node = temp_node->next;
		}
	}
return 1;
}