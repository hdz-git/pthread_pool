#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 1024


void * show_list(void * date)
{

    int num = *((int *)date);

    printf("date:%d\n",num);

}


void test01()
{ 
    list_t * my_list = Inint_list(); 
    int i,size;

    int a = 10;
    int b = 20;

    int arr[10] = {1,2,3,4,5,6,7,8,9,0};

    for(i = 0;i < 10;i++)
    {
        add_list(my_list,arr+i);
    }


    int temp = *((int*) del_list(my_list));

    printf("temp:%d\n",temp);

    print_list(my_list,show_list);

    printf("----------------------------------\n");

    add_list(my_list,&a);

    int temp1 = *((int*) del_list(my_list));

    printf("temp1:%d\n",temp1);
   // size = size_list(my_list);

   // printf("num:%d\n",size);

    print_list(my_list,show_list);


    free_list(my_list);


}

int main()
{
    test01();

    exit(0);
}
