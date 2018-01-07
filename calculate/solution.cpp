#include <iostream>
#include <string.h>

using namespace std;

#define MAX_LEN  (32+1)

//#define DBG


typedef enum
{
	FALSE,
	TRUE
}STATUS_T;

typedef struct
{
	int operate[MAX_LEN];//������������int���ַ����洢
	int len;//�������ĳ���
}operate_t;

typedef struct
{
	operate_t first_orig;//��һ����������ԭʼ����
	operate_t first_num;//��һ���������Ĳ�������
	char charact;//������
	operate_t second_orig;//�ڶ�����������ԭʼ����
	operate_t second_num;//�ڶ����������Ĳ�������
	operate_t dis_result;//�������
	int flag;

}calculate_t;

calculate_t calculate;

static void init(void)
{
	int i;
	/*��ʼ���������в������Ͳ��������*/	
	for (i = 0; i<MAX_LEN; i++)
	{
		calculate.first_orig.operate[i] = 0;
		calculate.first_num.operate[i] = 0;
		
		calculate.second_orig.operate[i] = 0;
		calculate.second_num.operate[i] = 0;
		
		calculate.dis_result.operate[i] = 0;
	}
	calculate.first_orig.len = 0;
	calculate.first_num.len = 0;

	calculate.second_orig.len = 0;
	calculate.second_num.len = 0;
	calculate.flag = FALSE;

	calculate.dis_result.len = 0;
	calculate.charact = '\0';
}
static void debug(void)
{
#ifdef DBG
	int i, j;
	printf("first num len:%d \n", calculate.first_num.len);

	for (i = 0; i<calculate.first_num.len; i++)
	{
		printf("%d ", calculate.first_num.operate[i]);
	}
	printf("\n");

	printf("second num len:%d \n", calculate.second_num.len);
	for (i = 0; i<calculate.second_num.len; i++)
	{
		printf("%d ", calculate.second_num.operate[i]);
	}
	printf("\n");
#endif
}

/*�Բ��������з�ת��ʹ֮��˳���ɵ��򵽸�������*/
static void rollover(void)
{
	int i, j, len = 0;
	int result[MAX_LEN];

	calculate.first_num.len = calculate.first_orig.len;
	for (i = 0; i<calculate.first_num.len; i++)
	{
		calculate.first_num.operate[calculate.first_num.len - i - 1] = calculate.first_orig.operate[i];
	}
	for (i = calculate.first_num.len; i<MAX_LEN; i++)
	{
		calculate.first_num.operate[i] = 0;
	}

	calculate.second_num.len = calculate.second_orig.len;
	for (i = 0; i<calculate.second_num.len; i++)
	{
		calculate.second_num.operate[calculate.second_num.len - i - 1] = calculate.second_orig.operate[i];
	}
	for (i = calculate.second_num.len; i<MAX_LEN; i++)
	{
		calculate.second_num.operate[i] = 0;
	}

	for (i = 0; i<MAX_LEN; i++)
	{
		calculate.dis_result.operate[i] = 0;
	}
	calculate.dis_result.len = 0;
}

static void store_result(void)
{
	int len = 0, i = MAX_LEN;
	while (calculate.dis_result.operate[i] == '\0')
	{
		i--;
		len++;
	}
	calculate.first_orig.len = MAX_LEN - len + 1;
	if (calculate.first_orig.len <= 0)
	{
		calculate.first_orig.len = 1;
	}
	for (i = 0; i < calculate.first_orig.len; i++)
	{
		calculate.first_orig.operate[calculate.first_orig.len - i - 1] = calculate.dis_result.operate[i];
	}

	calculate.dis_result.len = calculate.first_orig.len;
	for (i = 0; i< calculate.dis_result.len; i++)
	{
		calculate.dis_result.operate[i] = calculate.first_orig.operate[i];
	}

	calculate.flag = FALSE;
	calculate.second_orig.len = 0;
}

static void add(void)
{
	int i;
	int len, len_first, len_second;

	rollover();
	debug();
	len = (calculate.first_num.len>calculate.second_num.len) ? calculate.first_num.len : calculate.second_num.len;
	len_first = calculate.first_num.len;
	len_second = calculate.second_num.len;

	for (i = 0; i<len; i++)//ѭ�����
	{
		calculate.dis_result.operate[i] += calculate.first_num.operate[i];
		calculate.dis_result.operate[i] += calculate.second_num.operate[i];

		calculate.dis_result.operate[i + 1] += calculate.dis_result.operate[i] / 10;
		calculate.dis_result.operate[i] = calculate.dis_result.operate[i] % 10;
	}
	store_result();
}
static void odejmowanie(void)
{
	int i, len;

	rollover();
	debug();

	len = calculate.first_num.len;
	for (i = 0; i<len; i++)
	{
		if (i< calculate.second_num.len)//�������С�ڵڶ������ĳ���
		{
			if (calculate.first_num.operate[i] >= calculate.second_num.operate[i])//��һ�����ĵ�ǰλ���ڵڶ������ĵ�ǰλ��ֱ�Ӽ�
			{
				calculate.dis_result.operate[i] = calculate.first_num.operate[i] - calculate.second_num.operate[i];
			}
			else//��һ�����ĵ�ǰλС�ڵڶ������ĵ�ǰλ�����λ��λ֮���
			{
				calculate.dis_result.operate[i] = calculate.first_num.operate[i] + 10 - calculate.second_num.operate[i];
				calculate.first_num.operate[i + 1]--;
			}
		}
		else
		{
			if (calculate.first_num.operate[i] < 0)//�жϼ���������֮���һλ�Ƿ�С��0
			{
				calculate.dis_result.operate[i] += 10;
				calculate.first_num.operate[i + 1]--;
			}
		}
	}
	store_result();
}

static void mult(void)
{
	int i, j, len = 0;

	/*����Ϊ0���߱�����Ϊ0�� ���ҲΪ0*/
	if (((calculate.first_orig.len == 1) && (calculate.first_orig.operate[0] == 0)) || ((calculate.second_orig.len == 1) && (calculate.second_orig.operate[0] == 0)))
	{
		calculate.first_orig.len = 1;
		calculate.first_orig.operate[0] = 0;

		calculate.dis_result.len = 1;
		calculate.dis_result.operate[0] = 0;

		calculate.flag = FALSE;
		calculate.second_orig.len = 0;

		return;
	}

	rollover();
	debug();

	for (i = 0; i<calculate.first_num.len; i++)//�ӵ�λ����λѭ�����
	{
		for (j = 0; j<calculate.second_num.len; j++)
		{
			calculate.dis_result.operate[i + j] += (calculate.first_num.operate[i])*(calculate.second_num.operate[j]);
			calculate.dis_result.operate[i + j + 1] += calculate.dis_result.operate[i + j] / 10;
			calculate.dis_result.operate[i + j] = calculate.dis_result.operate[i + j] % 10;
		}
	}
	store_result();
}

static int substract(int* num1, int* num2, int len1, int len2)
{
	int i, j;

	if (len1 < len2)//��������ĳ���С�ڱ�������ֱ���˳�
	{
		return -1;
	}
	if (len1 == len2)//��������ͱ��������
	{
		for (i = len1 - 1; i >= 0; i--)//�жϼ����Ƿ���ڱ�����
		{
			if (num1[i] > num2[i])//���������
			{
				break;
			}
			else if (num1[i] < num2[i])//С�����˳�
			{
				return -1;
			}
		}
	}


	for (i = 0; i <= len1 - 1; i++)//�ӵ�λ���λѭ���ݼ�
	{
		num1[i] -= num2[i];
		if (num1[i] < 0)//���λ��λ
		{
			num1[i] += 10;
			num1[i + 1]--;
		}
	}
	for (i = len1 - 1; i >= 0; i--)
	{
			if (num1[i])
			{
				return (i + 1);//���ؼ���֮�����ĳ���
			}
	}
	
	return 0;
}

static void divis(void)
{
	int i, j;
	int result[MAX_LEN];
	int len_first, len_second, len_dif;
	int temp;
	int* num1, *num2;

	rollover();
	debug();

	len_first = calculate.first_num.len;
	len_second = calculate.second_num.len;
	if ((len_second == 1) && (calculate.second_num.operate[0] == 0))//���������Ϊ0��ֱ���˳�
	{
		return;
	}
	if ((len_first == 1) && (calculate.first_num.operate[0] == 0))//�������Ϊ0������Ϊ0
	{
		calculate.first_orig.len = 1;
		calculate.first_orig.operate[0] = 0;

		calculate.dis_result.len = 1;
		calculate.dis_result.operate[0] = 0;

		calculate.flag = FALSE;
		calculate.second_orig.len = 0;

		return;
	}

	num1 = calculate.first_num.operate;
	num2 = calculate.second_num.operate;

	if (len_first < len_second)//�������С�ڱ�������ֱ���˳�
	{
		return;
	}

	len_dif = len_first - len_second;//��������ͱ�����֮��ĳ��Ȳ�
	for (i = len_first - 1; i >= 0; i--)//�����������ƣ�ʹ��ͳ�������һ��
	{
		if (i >= len_dif)
		{
			calculate.second_num.operate[i] = calculate.second_num.operate[i - len_dif];//������������
		}
		else
		{
			calculate.second_num.operate[i] = 0;//��ʣ��λ��0
		}
	}
	len_second = calculate.second_num.len = len_first;

	for (i = 0; i <= len_dif; i++)//��0-len_dif��Χ�ڣ��Գ����ͱ�����ѭ������������
	{
		while ((temp = substract(num1, num2 + i, len_first, len_second - i)) >= 0)//�������֮��ʣ�೤�ȴ���0
		{
			len_first = temp;//���ó��ȸ�ֵ����һ����
			calculate.dis_result.operate[len_dif - i]++;//�������Ӧλ+1
		}
	}

	store_result();
}


void Run(char button, char display[32 + 1])
{
	int i;

	for (i = 0; i<MAX_LEN; i++)
	{
		display[i] = '\0';
	}
	if (button == 'C')
	{
		init();
		display[0] = '0';
		return;
	}
	if(calculate.charact == '\0')
	{
		if(button == '+' || button == '-' || button == '*' || button == '/' || button == '=')
		{
			calculate.charact = button;
		}
		else
		{
			calculate.first_orig.operate[calculate.first_orig.len++] = button - '0';
		}
		for (i = 0; i<calculate.first_orig.len; i++)
		{
			display[i] = calculate.first_orig.operate[i] + '0';
		}
	}

	else //not first num
	{
		if ((calculate.flag == TRUE) && (button == '+' || button == '-' || button == '*' || button == '/' || button == '='))
		{//have the first and second num
			if (calculate.charact == '+')
			{
				add();
			}
			else if (calculate.charact == '-')
			{
				odejmowanie();
			}
			else if (calculate.charact == '*')
			{
				mult();
			}
			else if (calculate.charact == '/')
			{
				divis();
			}
			if (button != '=')
			{
				calculate.charact = button;
			}
			else
			{
				calculate.charact = '\0';
			}

			for (i = 0; i< calculate.dis_result.len; i++)
			{
				display[i] = calculate.dis_result.operate[i] + '0';
			}
		}
		else  //second num
		{
			calculate.flag = TRUE;
			calculate.second_orig.operate[calculate.second_orig.len++] = button - '0';
			for (i = 0; i<calculate.second_orig.len; i++)
			{
				display[i] = calculate.second_orig.operate[i] + '0';
			}
		}
	}
}
