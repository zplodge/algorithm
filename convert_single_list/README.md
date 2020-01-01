1. # builid and run command is as follow:

  ./run.sh

2. ## test case type is as follow:
	
	1. calculate route distance with the route path
	
	2. calculate the route path with max hops
	
	3. calculate the route path with fixed hops
	
	4. calculate the shortest route form start to end
	
	5. calculate the route path with max distance
	
	   
	
3. ## 单链表队列

   1. head和tail初始值都为空；
   2. 初始化申请一块内存， head和tail都指向该内存块；
   3. 入队操作时新申请一块内存存储入队数据，链接到链表尾部，移动tail到链表尾部；
   4.  出队的时候弹出head->next指针；然后释放head内存；

   

   