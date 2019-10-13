#ifndef _MYLIST__H_
#define _MYLIST__H_

#define ERROR 0

//链式队列

typedef void list_t;

list_t * Inint_list();     //初始化

list_t * get_endlist(list_t * my_list);   //得到尾节点

int add_list(list_t * my_list,void * date);  //尾插

void * del_list(list_t * my_list);  //返回头节点的下一个节点数据并删除

void print_list(list_t * my_list,void*(*show_list)(void *)); //打印链表

int size_list(list_t * my_list);  //返回链表的元素个数

void free_list(list_t * my_list); //释放链表

int add_index_list(list_t * my_list,int index,void * date);  //指定位置插入元素，如果插入位置大于链表长度，则进行尾插




#endif
