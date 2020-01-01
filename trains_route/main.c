/*
 * @Author: Leon
 * @emial: zhangpeng@tuya.com
 * @Date: 2019-10-16 23:27:12
 * @LastEditors: Leon
 * @LastEditTime: 2019-12-08 14:37:46
 * @file name: main.c
 * @Description: this file is the main function handler file
 * @Copyright: 
 * @Company: 
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "queue.h"

typedef enum
{
	DISTANCE,
	DURATION,
}TRAINS_CAL_TYPE_E;

unsigned char trains_graph[MAX_INPUT_NODES][MAX_INPUT_NODES] = { 0 }; //source route graph
unsigned char trains_flag[MAX_INPUT_NODES][MAX_INPUT_NODES] = { 0 }; // source route graph flag
unsigned char test_case_input[MAX_INPUT_NODES][MAX_INPUT_NODES] = { 0 }; //test case type and rodes

/**
 * @description: convert the input path to trains graph
 * @param {path_str} route path string
 * @param {path_len} route path lendth
 * @return: none
 */
void load_path_to_graph(char* path_str, int path_len)
{
	int start = 0, end = 0, value = 0;

	if (path_len < 3)
	{
		printf("path error ! \n\r");
		return;
	}
	start = *path_str++ - 'A';
	end = *path_str++ - 'A';
	value = *path_str++ - '0';

	if (start < 0 || end < 0 || value < 0)
	{
		printf("input param error; \n\r");
		return;
	}

	trains_graph[start][end] = value;
}

/**
 * @description: load the input route from the test_input.txt file
 * @param {NULL} 
 * @return: none
 */
void load_input_route(void)
{
	int N;
	
	setbuf(stdout, NULL);
	freopen("test_input.txt", "r", stdin);

	if(scanf("%d", &N) != EOF)
	{
		for (int i = 0; i < MAX_INPUT_NODES; i++)
		{
			for (int j = 0; j < MAX_INPUT_NODES; j++)
			{
				trains_graph[i][j] = INVALID_PATH;
			}
		}

		for (int i = 0; i < N; i++)
		{
			char path[MAX_TEST_CASE_NODES] = { 0 };

			do
			{
				scanf("%s", path);
			} while (path[0] == 0);

			load_path_to_graph(path, MAX_TEST_CASE_NODES);
		}
	}

	fclose(stdin);
}

/**
 * @description: load the test case from the test_case.txt file
 * @param {NULL} 
 * @return: test case number
 */
int load_test_case(void)
{
	/*load test case*/
	/*
	1. calculate route distance with the route path
	2. calculate the route path with max hops
	3. calculate the route path with fixed hops
	4. calculate the shortest route form start to end
	5. calculate the route path with max distance
	*/
	int test_case = 0;

	freopen("test_case.txt", "r", stdin);
	if(scanf("%d", &test_case) == EOF)
	{
		printf("test case num is error");
		return 0;
	}

	for (int i = 0; i < test_case; i++)
	{
		for (int j = 0; j < test_case; j++)
		{
			test_case_input[i][j] = 0;
		}
	}

	for (int i = 0; i < test_case; i++)
	{
		int test_case_type = 0;

		if(scanf("%d", &test_case_type) == EOF)
		{
			printf("test case type is error");
			return 0;
		}

		test_case_input[i][0] = (char)test_case_type;

		if ((test_case_type == ROUTE_DISTANCE) || (test_case_type == SHORTEST_DISTANCE))
		{
			char path[MAX_TEST_CASE_NODES] = { 0 };

			do
			{
				scanf("%s", path);
			} while (path[0] == 0);
			strncpy((char*)&test_case_input[i][1], path, MAX_TEST_CASE_NODES - 1);
		}
		else if ((test_case_type == MIN_DISTANCE) || (test_case_type == EQUAL_DISTANCE) || (test_case_type == NUM_WITH_MAX_DISTANCE))
		{
			char path[MAX_TEST_CASE_NODES] = { 0 };
			int value = 0;
			
			scanf("%d", &value);
			test_case_input[i][1] = (char)value;

			do
			{
				scanf("%s", path);
			} while (path[0] == 0);
			strncpy((char*)&test_case_input[i][2], path, MAX_TEST_CASE_NODES - 1);
		}
		else
		{
			printf("test case type is error \n");
		}
	}

	fclose(stdin);

	return test_case;
}

/**
 * @description: get the distance form the source node  to destination node
 * @param {start} the start node
 * @param {end} the end node
 * @return: distance
 */
int get_distance_one_hops(int start, int end)
{
	return trains_graph[start][end];
}

/**
 * @description: calculate the distance accroding the route path
 * @param {path} the route path
 * @param {len} the route length
 * @return: distance
 */
int cal_route_distance_and_duration(unsigned char* path, int len)
{
	int distance = 0, result = 0;

	if (NULL == path)
	{
		return INVALID_PATH;
	}

	if (len <= 1)
	{
		return INVALID_PATH;
	}
	
	for (int i = 0; i < len - 1; i++)
	{
		if ((distance = get_distance_one_hops(*(path + i) - 'A', *(path + i + 1) - 'A')) < INVALID_PATH)
		{
				result += distance;
		}
		else
		{
			return INVALID_PATH;
		}
	}

	return result;
}

/**
 * @description: calculate the total route path with in the max hops
 * @param {max_hops} max hops of the route path
 * @param {start_node} the start node 
 * @param {end_node} the end node
 * @return: distance
 */
int cal_max_hops(int max_hops_or_duration, int start_node, int end_node)
{
	int result = 0;
	queue_list_t node_info;

	memset(&node_info, 0, sizeof(queue_list_t));
	node_info.node = start_node;
	node_info.distance = 0;
	node_info.hops = 0;

	if (TRUE != queue_push(&node_info))
	{
		printf("queue push not success");
		return INVALID_PATH;
	}

	while (head != tail)
	{
		queue_list_t node_info;

		if (TRUE != queue_pop(&node_info))
		{
			printf("queue pop not success");
			return INVALID_PATH;
		}

		if ((node_info.hops > 0) && (node_info.node == end_node))
		{
			result++;
		}

		if (node_info.hops < max_hops_or_duration)
		{
			for (int i = 0; i < MAX_INPUT_NODES; i++)
			{
				if (trains_graph[node_info.node][i] != INVALID_PATH)
				{
					queue_list_t current_node;

					memset(&current_node, 0, sizeof(queue_list_t));
					current_node.node = i;
					current_node.hops = node_info.hops + 1;
					current_node.distance += trains_graph[node_info.node][i];
					if (TRUE != queue_push(&current_node))
					{
						printf("queue push not success");
						return INVALID_PATH;
					}
				}
			}
		}
	}

	return result;
}

/**
 * @description: calculate the total route path with the hops
 * @param {hops} hops of the route path
 * @param {start_node} the start node 
 * @param {end_node} the end node
 * @return: distance
 */
int cal_equal_hops(int hops, int start_node, int end_node)
{
	int result = 0;
	queue_list_t node_info;

	memset(&node_info, 0, sizeof(queue_list_t));
	node_info.node = start_node;
	node_info.distance = 0;
	node_info.hops = 0;

	if (TRUE != queue_push(&node_info))
	{
		printf("queue push not success");
		return INVALID_PATH;
	}

	while (head != tail)
	{
		queue_list_t node_info;

		if (TRUE != queue_pop(&node_info))
		{
			printf("queue pop not success");
			return INVALID_PATH;
		}

		if ((node_info.hops == hops) && (node_info.node == end_node))
		{
			result++;
		}

		if (node_info.hops < hops)
		{
			for (int i = 0; i < MAX_INPUT_NODES; i++)
			{
				if (trains_graph[node_info.node][i] != INVALID_PATH)
				{
					queue_list_t current_node;

					memset(&current_node, 0, sizeof(queue_list_t));
					current_node.node = i;
					current_node.hops = node_info.hops + 1;
					current_node.distance += trains_graph[node_info.node][i];
					if (TRUE != queue_push(&current_node))
					{
						printf("queue push not success");
						return INVALID_PATH;
					}
				}
			}
		}
	}

	return result;
}

/**
 * @description: calculate the shortest route distance form the start to end
 * @param {start_node} the start node 
 * @param {end_node} the end node
 * @return: distance
 */
int cal_shortest_distance(int start_node, int end_node)
{
	int result = INVALID_PATH;
	queue_list_t node_info;
	static bool_t flag = FALSE;

	memset(&node_info, 0, sizeof(queue_list_t));

	node_info.node = start_node;
	node_info.distance = 0;
	node_info.hops = 0;
	node_info.flag = FALSE;

	if (TRUE != queue_push(&node_info))
	{
		printf("queue push not success");
		return INVALID_PATH;
	}

	memset(trains_flag, 0, MAX_INPUT_NODES*MAX_INPUT_NODES);

	while (head != tail)
	{
		queue_list_t node_info;

		if (TRUE != queue_pop(&node_info))
		{
			printf("queue pop not success");
			return INVALID_PATH;
		}

		if (start_node == end_node)
		{
			if (node_info.node == end_node)
			{
				if (flag == TRUE)
				{
					result = result < node_info.distance ? result : node_info.distance;
				}
				else
				{
					flag = TRUE;
				}
			}
		}
		else
		{
			if (node_info.node == end_node)
			{
				result = result < node_info.distance ? result : node_info.distance;
			}
		}

		for (int i = 0; i < MAX_INPUT_NODES; i++)
		{
			if ((trains_graph[node_info.node][i] != INVALID_PATH) && (trains_flag[node_info.node][i] == FALSE))
			{
				queue_list_t current_node;

				memset(&current_node, 0, sizeof(queue_list_t));
				current_node.node = i;
				current_node.hops = node_info.hops + 1;
				current_node.distance = node_info.distance + trains_graph[node_info.node][i];
				trains_flag[node_info.node][i] = TRUE;
				if (TRUE != queue_push(&current_node))
				{
					printf("queue push not success");
					return INVALID_PATH;
				}
			}
		}
	}

	flag = FALSE;
	return result;
}

/**
 * @description: calculate the total route distance with the max hops
 * @param {max_dis} max distance of the route path
 * @param {start_node} the start node 
 * @param {end_node} the end node
 * @return: distance
 */
int cal_hops_with_max_distance(int max_dis, int start_node, int end_node)
{
	int result = 0;
	queue_list_t node_info;

	memset(&node_info, 0, sizeof(queue_list_t));

	node_info.node = start_node;
	node_info.distance = 0;
	node_info.hops = 0;

	if (TRUE != queue_push(&node_info))
	{
		printf("queue push not success");
		return INVALID_PATH;
	}

	while (head != tail)
	{
		queue_list_t node_info;

		if (TRUE != queue_pop(&node_info))
		{
			printf("queue pop not success");
			return INVALID_PATH;
		}

		if ((node_info.distance < max_dis) && (node_info.node == end_node))
		{
			result++;
		}

		if (node_info.distance < max_dis)
		{
			for (int i = 0; i < MAX_INPUT_NODES; i++)
			{
				if (trains_graph[node_info.node][i] != INVALID_PATH)
				{
					queue_list_t current_node;

					memset(&current_node, 0, sizeof(queue_list_t));
					current_node.node = i;
					current_node.hops = node_info.hops + 1;
					current_node.distance = node_info.distance + trains_graph[node_info.node][i];
					if (TRUE != queue_push(&current_node))
					{
						printf("queue push not success");
						return INVALID_PATH;
					}
				}
			}
		}
	}

	if (start_node == end_node) //if start node is same with end node, the result shoud be reduce 1
	{
		result--;
	}
	return result;
}

void cal_trains_distance(int start, int end)
{
	int value = 0;

	for (int i = start; i < end; i++)
	{
		queue_init();

		switch (test_case_input[i][0])
		{
		case ROUTE_DISTANCE:
			value = cal_route_distance_and_duration(&test_case_input[i][1], strnlen((char*)&test_case_input[i][1], MAX_TEST_CASE_NODES));
			break;

		case MIN_DISTANCE: //trips within max hops 
			value = cal_max_hops(test_case_input[i][1], test_case_input[i][2] - 'A', test_case_input[i][3] - 'A');
			break;

		case EQUAL_DISTANCE:
			value = cal_equal_hops(test_case_input[i][1], test_case_input[i][2] - 'A', test_case_input[i][3] - 'A');
			break;

		case SHORTEST_DISTANCE:
			value = cal_shortest_distance(test_case_input[i][1] - 'A', test_case_input[i][2] - 'A');
			break;

		case NUM_WITH_MAX_DISTANCE:
			value = cal_hops_with_max_distance(test_case_input[i][1], test_case_input[i][2] - 'A', test_case_input[i][3] - 'A');
			break;

		default:
			printf("test case tyope error \n");
			break;
		}

		queue_deinit();
		printf("Output #%d: ", i + 1);
		if (value !=  INVALID_PATH)
		{
			printf("%d \n", value);
		}
		else
		{
			printf("NO SUCH ROUTE \n");
		}
	}
}

int main(void)
{
	int test_case_num = 0;

	load_input_route(); //load input route path
	test_case_num = load_test_case(); // load test case 
	
	if (test_case_num > 0)
	{
		cal_trains_distance( 0,9); //caculate the part 1 result
	}

	if (test_case_num > 0)
	{
		// cal_trains_durations(10, 19); //caculate the part 2 result
	}

	return 0;
}
