#include "thread.h"
#include <stdlib.h>
#include <stdio.h>

#define NUMSIZE 3
#define MIN 30000000
#define MAX MIN+200

void * show_primer(void * arg)
{
    int temp = *((int *)arg);
    int flag = 1;

    for(int i=2;i<temp;i++)
    {
        if(temp % i == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag)
    {
        printf("%d\n",temp);
    }

    return NULL;

}


void test01()
{
    int arr[MAX-MIN+1];

    pthreadpool_t * pool = Inint_pthread_pool(NUMSIZE);
    
    for(int i=MIN;i<MAX;i++)
    {
        arr[i-MIN] = i;

        add_task(pool,&arr[i-MIN],show_primer);
    }

    free_ptherad(pool);
}





int main()
{
    test01();

    exit(0);
}
