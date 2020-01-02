1. # 单链表翻转

   1. 定义三个指针， 分别标记上一个元素（prev）、当前元素(current)和临时变量(temp)；

   2. 初始化当前元素为链表的head地址； 上一个元素(prev)和临时变量(temp)为空；

   3. 当当前元素(current)不为空的时候，临时变量(temp)指向当前元素(current)， 临时变量(temp)的下一个元素指向上一个元素(prev);

   4. 当前元素(current)指向当前元素(current)的下一个元素，上一个元素（prev）指向临时变量的地址；

   5. 此时翻转即完成；

      ```
      while(current)
      
      {
      
      ​	temp = current;
      
      ​	temp->next = prev;
      
      ​	current = current->next;
      
      ​	prev = temp;
      
      }
      ```

      



