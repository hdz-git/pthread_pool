#include "queue_list.h"
#include <stdlib.h>
#include <stdio.h>

void * show_queue(void * date)
{
    int num = *((int *)date);

    printf("%d\n",num);
}

void test01()
{
    queue_t * queue_list = Inint_queue();

    for(int i=0;i<10;i++)
    {
        add_queue(queue_list,&i);
    }

    print_queue(queue_list,show_queue);
}


int main()
{
    test01();

    exit(0);
}
