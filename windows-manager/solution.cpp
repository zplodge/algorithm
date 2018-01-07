#define MAX_WIN 500
#define WIDTH   160
#define HEIGHT  120

extern void DisplayUpdate(char dispbuf[HEIGHT][WIDTH]);

char DisBuf[HEIGHT][WIDTH];
int DisIdx[HEIGHT][WIDTH];

int background_color;
int window_cnt;
struct WindowInfo
{
	int parent;
	int x;
	int y;
	int width;
	int height;
	char bgcolor;
	int child[MAX_WIN];
	int start_row;
	int start_col;
	int end_row;
	int end_col;
	char color[HEIGHT][WIDTH];
} window_info[MAX_WIN];

int last_display_cnt, new_display_cnt;
int last_display_order[MAX_WIN], new_display_order[MAX_WIN];

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

void update_DisBuf()
{
	int cnt, i, j;
	int start_row, start_col, end_row, end_col;
	int idx;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			DisBuf[i][j] = background_color;
			DisIdx[i][j] = -1;
		}
	}
	for (cnt = 0; cnt < new_display_cnt; cnt++)
	{
		idx = new_display_order[cnt];

		for (i = window_info[idx].start_row; i < window_info[idx].end_row; i++)
		{
			for (j = window_info[idx].start_col; j < window_info[idx].end_col; j++)
			{
				DisBuf[i][j] = window_info[idx].color[i][j];
				DisIdx[i][j] = idx;
			}
		}
	}
}

void InitWinManager(char bgcolor)
{
	// To do
	int i, j;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			DisBuf[i][j] = bgcolor;
		}
	}
	window_cnt = 0;
	background_color = bgcolor;
	last_display_cnt = 0;
	new_display_cnt = 0;
	//Do not remove
	DisplayUpdate(DisBuf);
}

int CreateWindow(int parent, int x, int y, int width, int height, char bgcolor)
{
	// To do
	int i, j;
	window_info[window_cnt].parent = parent;
	window_info[window_cnt].width = width;
	window_info[window_cnt].height = height;
	window_info[window_cnt].bgcolor = bgcolor;
	window_info[window_cnt].child[0] = 0;
	if (parent == -1)
	{
		window_info[window_cnt].x = x;
		window_info[window_cnt].y = y;
		window_info[window_cnt].start_row = max(window_info[window_cnt].y, 0);
		window_info[window_cnt].start_col = max(window_info[window_cnt].x, 0);
		window_info[window_cnt].end_row = min(window_info[window_cnt].y + window_info[window_cnt].height, HEIGHT);
		window_info[window_cnt].end_col = min(window_info[window_cnt].x + window_info[window_cnt].width, WIDTH);

		last_display_order[last_display_cnt] = window_cnt;
		last_display_cnt++;
		new_display_cnt = last_display_cnt;
		for (i = 0; i < last_display_cnt; i++)
		{
			new_display_order[i] = last_display_order[i];
		}
	}
	else
	{
		int pre_window;
		window_info[window_cnt].x = x + window_info[parent].x;
		window_info[window_cnt].y = y + window_info[parent].y;
		window_info[window_cnt].start_row = max(window_info[window_cnt].y, window_info[parent].start_row);
		window_info[window_cnt].start_col = max(window_info[window_cnt].x, window_info[parent].start_col);
		window_info[window_cnt].end_row = min(window_info[window_cnt].y + window_info[window_cnt].height, window_info[parent].end_row);
		window_info[window_cnt].end_col = min(window_info[window_cnt].x + window_info[window_cnt].width, window_info[parent].end_col);

		window_info[parent].child[0]++;
		window_info[parent].child[window_info[parent].child[0]] = window_cnt;
		if (window_info[parent].child[0] == 1)
		{
			pre_window = parent;
		}
		else
		{
			pre_window = window_info[parent].child[window_info[parent].child[0] - 1];
			while (window_info[pre_window].child[0] != 0)
			{
				pre_window = window_info[pre_window].child[window_info[pre_window].child[0]];
			}
		}

		for (i = 0; i < last_display_cnt; i++)
		{
			new_display_order[i] = last_display_order[i];
		}
		for (i = last_display_cnt - 1; i >= 0; i--)
		{
			if (new_display_order[i] == pre_window)
			{
				new_display_order[i + 1] = window_cnt;
				break;
			}
			new_display_order[i + 1] = new_display_order[i];
		}
		last_display_cnt++;
		new_display_cnt = last_display_cnt;
		for (i = 0; i < new_display_cnt; i++)
		{
			last_display_order[i] = new_display_order[i];
		}
	}

	for (i = window_info[window_cnt].start_row; i < window_info[window_cnt].end_row; i++)
	{
		for (j = window_info[window_cnt].start_col; j < window_info[window_cnt].end_col; j++)
		{
			window_info[window_cnt].color[i][j] = bgcolor;
		}
	}

	int mHandle = window_cnt;
	window_cnt++;

	update_DisBuf();
	//Do not remove
	DisplayUpdate(DisBuf);

	return mHandle;
}

void DestroyWindow(int handle)
{
	// To do
	if (window_info[handle].parent != -1)
	{
		int parent = window_info[handle].parent;
		int i, j;
		for (i = 1; i <= window_info[parent].child[0]; i++)
		{
			if (window_info[parent].child[i] == handle)
			{
				break;
			}
		}
		for (j = i + 1; j <= window_info[parent].child[0]; j++)
		{
			window_info[parent].child[j - 1] = window_info[parent].child[j];
		}
		window_info[parent].child[0]--;
	}

	int flag[MAX_WIN];
	int queue[MAX_WIN];
	int head, tail;
	int i, idx;
	for (i = 0; i < MAX_WIN; i++)
	{
		flag[i] = 0;
	}

	head = 0;
	tail = 0;
	flag[handle] = 1;
	queue[tail++] = handle;

	while (head != tail)
	{
		idx = queue[head++];
		for (i = 1; i <= window_info[idx].child[0]; i++)
		{
			flag[window_info[idx].child[i]] = 1;
			queue[tail++] = window_info[idx].child[i];
		}
	}

	new_display_cnt = 0;
	for (i = 0; i < last_display_cnt; i++)
	{
		if (flag[last_display_order[i]] == 0)
		{
			new_display_order[new_display_cnt] = last_display_order[i];
			new_display_cnt++;
		}
	}

	for (i = 0; i < new_display_cnt; i++)
	{
		last_display_order[i] = new_display_order[i];
	}
	last_display_cnt = new_display_cnt;

	update_DisBuf();

	//Do not remove
	DisplayUpdate(DisBuf);
}

void FillRect(int handle, int x, int y, int width, int height, char color)
{
	// To do
	int start_row, start_col, end_row, end_col;
	int i, j;
	start_row = max(window_info[handle].start_row, window_info[handle].y + y);
	start_col = max(window_info[handle].start_col, window_info[handle].x + x);
	end_row = min(window_info[handle].y + y + height, window_info[handle].end_row);
	end_col = min(window_info[handle].x + x + width, window_info[handle].end_col);

	for (i = start_row; i < end_row; i++)
	{
		for (j = start_col; j < end_col; j++)
		{
			window_info[handle].color[i][j] = color;
		}
	}

	update_DisBuf();
	//Do not remove
	DisplayUpdate(DisBuf);
}

void ActiveWindow(int handle)
{
	// To do
	int flag[MAX_WIN];
	int queue[MAX_WIN];
	int head, tail;
	int i, idx;
	for (i = 0; i < MAX_WIN; i++)
	{
		flag[i] = 0;
	}

	head = 0;
	tail = 0;

	int parent = handle;
	while (window_info[parent].parent != -1)
	{
		parent = window_info[parent].parent;
	}
	flag[parent] = 1;
	queue[tail++] = parent;

	while (head != tail)
	{
		idx = queue[head++];
		for (i = 1; i <= window_info[idx].child[0]; i++)
		{
			flag[window_info[idx].child[i]] = 1;
			queue[tail++] = window_info[idx].child[i];
		}
	}

	new_display_cnt = 0;
	for (i = 0; i < last_display_cnt; i++)
	{
		if (flag[last_display_order[i]] == 0)
		{
			new_display_order[new_display_cnt] = last_display_order[i];
			new_display_cnt++;
		}
	}
	for (i = 0; i < last_display_cnt; i++)
	{
		if (flag[last_display_order[i]] == 1)
		{
			new_display_order[new_display_cnt] = last_display_order[i];
			new_display_cnt++;
		}
	}
	for (i = 0; i < new_display_cnt; i++)
	{
		last_display_order[i] = new_display_order[i];
	}

	update_DisBuf();
	//Do not remove
	DisplayUpdate(DisBuf);
}

void Click(int x, int y)
{
	// To do
	if (DisIdx[y][x] != -1)
	{
		ActiveWindow(DisIdx[y][x]);
	}

	//Do not remove
	DisplayUpdate(DisBuf);
}