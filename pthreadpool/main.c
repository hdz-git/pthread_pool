#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pthreadpool.h"
#include "mylist.h"

#define MAXTASK 1
#define MAXPTHREAD 3

#define MIN 30000000
#define MAX MIN+200

static void * show_primer(void * arg)
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
    pthreadpool_t * pool = Inint_pthread_pool(MAXPTHREAD,MAXTASK);

    int arr[MAX - MIN +1];

    int i;

    for(int i = MIN;i < MAX;i++)
    {
        arr[i - MIN] = i;

        add_task(pool,(void *)&arr[i - MIN],show_primer);
    }

    sleep(5);

    free_ptherad(pool);

}



int main()
{
    test01();

    exit(0);
}
