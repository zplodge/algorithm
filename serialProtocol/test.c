#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include<unistd.h>
#include "sort.h"

//#define DBG

#define MAX_NUM   10000001
#define MAX_RAND  10

#if 0
struct timeval{
long tv_sec; /*秒*/
long tv_usec; /*微秒*/
};
timezone 结构定义为:
struct timezone{
int tz_minuteswest; /*和Greenwich 时间差了多少分钟*/
int tz_dsttime; /*日光节约时间的状态*/
};
#endif

int get_short_time(void)
{
    time_t timer;
	
    struct tm *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    printf("Local time is: %s\n", asctime(tblock));
    return 0;
	
}
int get_long_time(struct timeval *time)
{
	//struct timezone tz;
	//memset(&tz, 0, sizeof(struct timezone));
	
	if(NULL == time)
	{
		return 0;
	}
	gettimeofday(time, NULL);
	
	return 1;
}

int print_result(int* arry, int len)
{
#ifdef DBG
	printf("sort result check\n\r");
	int i = 0;
	for(i=0; i<len; i++)
	{
		printf("%d ",arry[i]);
	}
#endif
	while(len > 1)
	{
		if(arry[len-1] < arry[len-2])
		{
			printf("sort error, %d \n\r", len-1);
			return 0;
		}
		len--;
	}
	//printf("sort result check is ok \n\r");
	return 1;
}

static int sort(int sort_name, int* arry, int len)
{
	struct timeval start_time, end_time;
	int  sort_time = 0;
	get_long_time(&start_time);

	switch(sort_name)
	{
		case QUICK_SORT:
			quick_sort(arry, 0, len);
			printf("quick sort ");
		break;
		
		case HEAP_SORT:
			heap_sort(arry, len);
			printf("heap  sort ");
		break;
		
		case MERGE_SORT:
			merge_sort(arry, 0, len);
			printf("merge sort ");
		break;
		
		default:
		break;
	}
	
	get_long_time(&end_time);
	sort_time = (end_time.tv_sec - start_time.tv_sec)*1000000 + end_time.tv_usec - start_time.tv_usec;
	printf("time:%d us \n\r", sort_time);
	print_result(arry, MAX_NUM);
	return sort_time;
}
int  main(void)
{
	int i=0, j=0;
	int quick_sort_average_time = 0, heap_sort_everage_time = 0, merge_sort_everage_time = 0;
	int *arry = NULL, *sort_arry = NULL;
	struct timeval start_time, end_time;
	struct timeval tp;
	
	memset(&start_time, 0, sizeof(struct timeval));
	memset(&end_time, 0, sizeof(struct timeval));
	
	if(NULL == (arry = malloc(sizeof(int)*MAX_NUM)))
	{
		return 0;
	}
	if(NULL == (sort_arry = malloc(sizeof(int)*MAX_NUM)))
	{
		free(arry);
		return 0;
	}
	
	while(j < MAX_RAND)
	{
		i = 0;
		int quick_sort_time = 0, heap_sort_time = 0, merge_sort_time = 0;
		
		gettimeofday(&tp , NULL);
		srand(tp.tv_usec);
		while(i < MAX_NUM)
		{
			arry[i++] = (1 + (int)(100000000.0*rand()/(RAND_MAX+1.0)));
		}
		
		memcpy(sort_arry, arry, sizeof(int)*(MAX_NUM-1));
		quick_sort_time = sort(QUICK_SORT, arry, MAX_NUM-1);
		quick_sort_average_time += quick_sort_time;
		
		memcpy(sort_arry, arry, sizeof(int)*(MAX_NUM-1));
		heap_sort_time = sort(HEAP_SORT, arry, MAX_NUM);
		heap_sort_everage_time += heap_sort_time;

		
		memcpy(sort_arry, arry, sizeof(int)*(MAX_NUM-1));
		merge_sort_time = sort(MERGE_SORT, arry, MAX_NUM);	
		merge_sort_everage_time += merge_sort_time;
		
		j++;
	}
	
	printf("quick sort average time:%d\n\r", quick_sort_average_time/(MAX_RAND-1));
	printf("heap  sort average time:%d\n\r", heap_sort_everage_time/(MAX_RAND-1));
	printf("merge sort average time:%d\n\r", merge_sort_everage_time/(MAX_RAND-1));
	free(arry);
	free(sort_arry);
	
	return 1;
}



