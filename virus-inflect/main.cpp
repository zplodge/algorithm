//main
#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
#include <stdio.h>
#include <string.h>

#define CMD_ADD			1
#define CMD_MOVE		2
#define CMD_INFECT		3
#define CMD_RECOVER		4
#define CMD_REMOVE		5

extern void init();
extern int add(int id, int pid, int fileSize);
extern int move(int id, int pid);
extern int infect(int id);
extern int recover(int id);
extern int remove(int id);

static int run()
{
	int score = 0;
	int N, checkSum;
	int id, pid, fileSize;
	scanf("%d", &N);
	for (int i = 0; i < N; i++)
	{
		int cmd;
		scanf("%d", &cmd);
		int ret = 0;
		switch (cmd)
		{
		case CMD_ADD:
			scanf("%d %d %d", &id, &pid, &fileSize);
			ret = add(id, pid, fileSize);
			break;
		case CMD_MOVE:
			scanf("%d %d", &id, &pid);
			ret = move(id, pid);
			break;
		case CMD_INFECT:
			scanf("%d", &id);
			ret = infect(id);
			break;
		case CMD_RECOVER:
			scanf("%d", &id);
			ret = recover(id);
			break;
		case CMD_REMOVE:
			scanf("%d", &id);
			ret = remove(id);
			break;
		default:
			break;
		}
		scanf("%d", &checkSum);
		if (2560 == checkSum)
		{
			int result;
			result = checkSum;
		}
		if (ret == checkSum) score++;
	}
	return score;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T;
	scanf("%d", &T);

	int TotalScore = 0;
	for (int tc = 1; tc <= T; tc++)
	{
		init();
		int Score = run();
		TotalScore += Score;
		printf("#%d %d\n", tc, Score);
	}
	printf("TotalScore = %d\n", TotalScore);
	return 0;
}
