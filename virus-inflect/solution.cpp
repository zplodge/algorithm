/////////////////////////////////
#include <stdio.h>

#define CMD_ADD			1
#define CMD_MOVE		2
#define CMD_INFECT		3
#define CMD_RECOVER		4
#define CMD_REMOVE		5

#define INVALID_PAR     10001
#define MAX_NUM         10001
//#define NULL (TreeNODE *)0
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	FALSE,
	TRUE
}_STATUS_M;

//suggested data structure
struct TreeNODE
{
	TreeNODE *parent;
	TreeNODE *first_child;
	TreeNODE *prev_brother;
	TreeNODE *next_brother;

	int index;
	int flag;
	int id;
	int type;		// file or folder
	int size;		// original size
	int increase;	// increased size after infected
	int file_cnt;	// total number of files if type is folder or 1 if type is file 
} Tree[MAX_NUM];

struct TreeNODE *tree_root = NULL;

//suggested operations
void insert_node_to_tree(struct TreeNODE *new_node, struct TreeNODE *root)
{
	struct TreeNODE * index;

	if (new_node == NULL || root == NULL)
	{
		return;
	}
	new_node->parent = root;
	index = root->first_child;
	if (index == NULL)//first child
	{
		root->first_child = new_node;
	}
	else//not first child
	{
		while (index->next_brother != NULL)
		{
			index = index->next_brother;
		}
		index->next_brother = new_node;
		new_node->prev_brother = index;
		new_node->next_brother = NULL;
	}

	//update all parent info
	index = new_node->parent;
	while (index != NULL)
	{
		index->file_cnt += new_node->file_cnt;
		index->size += new_node->size;
		index = index->parent;
	}
}

static void free_node(struct TreeNODE *node)
{
	//struct TreeNODE *index = NULL;

	//index = node->first_child; 
	if (node == NULL)
	{
		return;
	}

	if (node->next_brother != NULL)
	{
		free_node(node->next_brother);
	}
	if (node->first_child != NULL)
	{
		free_node(node->first_child);
	}

	Tree[node->index].parent = NULL;
	Tree[node->index].first_child = NULL;
	Tree[node->index].prev_brother = NULL;
	Tree[node->index].next_brother = NULL;

	Tree[node->index].flag = FALSE;

	Tree[node->index].file_cnt = 0;
	Tree[node->index].id = 0;
	Tree[node->index].increase = 0;
	Tree[node->index].size = 0;

	if (node->index == 0)
	{
		Tree[node->index].id = 10000;
	}
	//Tree[node->index].index = 0;
}

void remove_node_from_tree(int flag, struct TreeNODE *node)
{
	struct TreeNODE *index = NULL, *first_child = NULL;

	//just remove the node, not  free the tree array	
	first_child = node->parent->first_child;
	if (node->parent->first_child->id == node->id)
	{
		node->parent->first_child = node->parent->first_child->next_brother;
		node->prev_brother = NULL;
		node->next_brother = NULL;
	}
	else
	{
		first_child = node->parent->first_child;
		while ((first_child != NULL) && (first_child->id != node->id))
		{
			first_child = first_child->next_brother;
		}
		first_child->prev_brother->next_brother = first_child->next_brother;

		if (first_child->next_brother != NULL)
		{
			first_child->next_brother->prev_brother = first_child->prev_brother;
		}

		first_child->prev_brother = NULL;
		first_child->next_brother = NULL;

	}

	index = node->parent;//update parent info
	while (index != NULL)
	{
		index->file_cnt -= node->file_cnt;
		index->size -= node->size;
		index = index->parent;
	}

	node->parent = NULL;

	if (flag == TRUE)//remove  the node and free the tree array
	{
		free_node(node);
	}
}

static int find_entry_from_tree(struct TreeNODE * node, int id)
{
	struct TreeNODE *index = node, *temp = NULL;
	int result = INVALID_PAR;

	if (index == NULL)
	{
		return INVALID_PAR;
	}
	if (index->id == id)
	{
		result = node->index;
		return result;
	}
	if (index->next_brother != NULL)
	{
		result = find_entry_from_tree(index->next_brother, id);
		if (result != INVALID_PAR)
		{
			return result;
		}
	}
	if (index->first_child != NULL)
	{
		result = find_entry_from_tree(index->first_child, id);
		if (result != INVALID_PAR)
		{
			return result;
		}
	}
	return INVALID_PAR;
}
TreeNODE *find_node_from_tree(struct TreeNODE * node, int id)
{
	struct TreeNODE *index = node, *temp = NULL;

	int result = INVALID_PAR;

	if (index == NULL)
	{
		return NULL;
	}
	if (id == 10000)
	{
		return &Tree[0];
	}
	result = find_entry_from_tree(node, id);
	if (result != INVALID_PAR)
	{
		return &Tree[result];
	}

	//return  &Tree[result];
}

static void update_size(struct TreeNODE * node, int size)
{
	if (node == NULL)
	{
		return;
	}

	if (node->next_brother != NULL)
	{
		update_size(node->next_brother, size);
	}
	if (node->first_child != NULL)
	{
		update_size(node->first_child, size);
	}


	if (node->type)//file
	{
		struct TreeNODE *index = NULL;

		node->increase = size;

		index = node->parent;
		while (index != NULL) //update all parent increase size
		{
			index->increase += node->increase;
			index = index->parent;
		}
	}
}

static void clear_increase(struct TreeNODE * node)
{
	if (node == NULL)
	{
		return;
	}

	if (node->next_brother != NULL)
	{
		clear_increase(node->next_brother);
	}
	if (node->first_child != NULL)
	{
		clear_increase(node->first_child);
	}
	node->increase = 0;
}

static void clear_size(struct TreeNODE * node)
{
	struct TreeNODE *index = NULL;
	int current_inc = 0;

	if (node == NULL)
	{
		return;
	}
	index = node->parent;
	current_inc = node->increase;
	while (index != NULL)
	{
		index->increase -= current_inc;
		index = index->parent;
	}
	node->increase = 0;
	if (node->first_child != NULL)
	{
		clear_increase(node->first_child);
	}
}

//interfaces to implement for problem solving
void init()
{
	int i, j;
	tree_root = &Tree[0];

	for (i = 1; i<MAX_NUM; i++)
	{
		Tree[i].parent = NULL;
		Tree[i].first_child = NULL;
		Tree[i].prev_brother = NULL;
		Tree[i].next_brother = NULL;

		Tree[i].flag = FALSE;
		Tree[i].index = 0;
		Tree[i].id = 0;
		Tree[i].type = 0;
		Tree[i].size = 0;
		Tree[i].increase = 0;
		Tree[i].file_cnt = 0;
	}

	Tree[0].parent = NULL;//root
	Tree[0].first_child = NULL;
	Tree[0].prev_brother = NULL;
	Tree[0].next_brother = NULL;

	Tree[0].flag = TRUE;
	Tree[0].index = 0;
	Tree[0].id = 10000;
	Tree[0].size = 0;
	Tree[0].type = 00;
	Tree[0].increase = 0;
	Tree[0].file_cnt = 0;
}

int add(int id, int pid, int fileSize)
{
	int i;
	struct TreeNODE *parent_node = NULL;

	for (i = 1; i<MAX_NUM; i++)
	{
		if (Tree[i].flag == FALSE)
		{
			break;
		}
	}

	Tree[i].first_child = NULL;
	Tree[i].prev_brother = NULL;
	Tree[i].next_brother = NULL;

	Tree[i].flag = TRUE;
	Tree[i].index = i;
	Tree[i].id = id;
	Tree[i].size = fileSize;
	Tree[i].type = (Tree[i].size > 0) ? 1 : 0;
	Tree[i].increase = 0;
	Tree[i].file_cnt = (Tree[i].size > 0) ? 1 : 0;

	parent_node = find_node_from_tree(tree_root, pid);
	insert_node_to_tree(&Tree[i], parent_node);

	return (parent_node->size + parent_node->increase);
}

int move(int id, int pid)
{
	struct TreeNODE *child_node = NULL, *parent_node = NULL;

	child_node = find_node_from_tree(tree_root, id);
	parent_node = find_node_from_tree(tree_root, pid);

	remove_node_from_tree(FALSE, child_node);
	insert_node_to_tree(child_node, parent_node);

	return (parent_node->size + parent_node->increase);
}

int infect(int id)
{
	struct TreeNODE *node = NULL;
	int size = 0;

	node = find_node_from_tree(tree_root, id);
	if (tree_root->file_cnt > 0)
	{
		size = (tree_root->size + tree_root->increase) / (tree_root->file_cnt);
	}
	else
	{
		return 0;
	}


	if (node->type == 1)
	{
		struct TreeNODE *index = NULL;

		node->increase += size;

		index = node->parent;
		while (index != NULL) //update all parent increase size
		{
			index->increase += node->increase;
			index = index->parent;
		}
	}
	else
	{
		if (node->first_child != NULL)
		{
			update_size(node->first_child, size);
		}
	}


	return (node->size + node->increase);
}

int recover(int id)
{
	struct TreeNODE *node = NULL;
	node = find_node_from_tree(tree_root, id);

	clear_size(node);
	return (node->size + node->increase);
}

int remove(int id)
{
	struct TreeNODE *node = NULL;
	int size = 0;

	node = find_node_from_tree(tree_root, id);

	size = node->size + node->increase;
	remove_node_from_tree(TRUE, node);
	return size;
}