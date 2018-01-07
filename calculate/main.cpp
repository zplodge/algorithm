//main.c
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

char button;
// int dummy1[...];
char number[32 + 1];
// int dummy2[...];
char compare[32 + 1];
// int dummy3[...];
char display[32 + 1];

void Run(char button, char display[32 + 1]);

int main(void)
{
	freopen("sample_input.txt", "r", stdin);

	int testcase;
	cin >> testcase;
	for (int t = 1; t <= testcase; ++t)
	{
		int cnt;
		cin >> cnt;
		int ret = 0;
		for (int i = 0; i < cnt; ++i)
		{
			cin >> button >> number;
			if (button == 'N')
			{
				int len = strlen(number);
				for (int j = 0; j < len; ++j)
				{
					compare[j] = number[j];
					compare[j + 1] = 0;
					Run(number[j], display);
					if (strcmp(display, compare) == 0)
						++ret;
				}
			}
			else
			{
				Run(button, display);
				if (strcmp(display, number) == 0)
					++ret;
			}
		}
		cout << "#" << t << " " << ret << endl;
	}
	return 0;
}
