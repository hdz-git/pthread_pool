#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylist.h"



typedef struct list_str
{
    struct list_str * front;
    void *date;
    struct list_str * next;
}list_st;

list_t * Inint_list()
{
    list_st * my_list = (list_st *)malloc(sizeof(list_st));


    if(my_list == NULL)
    {
        perror("malloc()");

        return NULL;
    }

    
    my_list->next = NULL;
    my_list->front = NULL;
    my_list->date = NULL;

    return (list_t*)my_list;

}

list_t * get_endlist(list_t * my_list)
{
    if(my_list == NULL)
    {
        return NULL;
    }

    list_st * m_list = (list_st *)my_list;

    list_st * temp = m_list;

    list_st * temp_list = NULL;

    while(temp != NULL)
    {
        temp_list = temp;
        temp = temp -> next;
    }

    return (list_t*)temp_list;

}

int add_list(list_t * my_list,void * date)
{
    if(my_list == NULL || date == NULL)
    {
        return -1;
    }
    list_t * temp_list = my_list;

    list_st * end_list = (list_st *)get_endlist(temp_list);

    list_st * new_list = (list_st *)malloc(sizeof(*new_list));

    if(new_list == NULL)
    {
        return -2;
    }


    new_list->date = date;
    
    new_list->next = end_list->next;

    new_list->front = end_list;

   /* if(end_list->next != NULL)
    {
        end_list->next->front = new_list;
    }
*/
    end_list->next = new_list;
    return 0;

}

void * del_list(list_t * my_list)
{
    if(my_list == NULL)
    {
        return ERROR;
    }

    list_st * m_list = (list_st *)my_list;

    if(m_list->next == NULL)
    {
        return ERROR;
    }

    else
    {
        list_st * temp = m_list->next; 

        list_st * temp_list = temp;
        
        if(temp->next != NULL)
        {
            temp->next->front = m_list;
        }
        temp->front->next = temp->next;

        void * num = temp_list->date;
    
        free(temp_list);

        return num;

    }
}

int size_list(list_t * my_list)
{
    if(my_list == NULL)
    {
        return -1;
    }

    int size = 0;

    list_st * m_list = (list_st *)my_list;

    list_st * temp_list = m_list->next;

    while(temp_list != NULL)
    {
        size++;

        temp_list = temp_list->next;
    }

    return size;

}

int add_index_list(list_t * my_list,int index,void * date)
{
    if(my_list == NULL || date == NULL)
    {
        return -1;
    }

    int size;
    int num = 0;

    if((size = size_list(my_list)) < index)
    {
        add_list(my_list,date);
    }
    else
    {
        list_st * m_list = (list_st *)my_list;

        list_st * temp_list = m_list->next;

        list_st * new_list = (list_st *)malloc(sizeof(list_st));

        if(new_list == NULL)
        {
            return -2;
        }

        new_list->date = date;

        while(temp_list != NULL)
        {
            if(num == (index - 1))
            {
                break;
            }
            num++;
            temp_list = temp_list->next;
        }

        temp_list->front->next = new_list;

        temp_list->front = new_list->front;

        new_list->next = temp_list;

        temp_list->front = new_list;
    }
    return 0;
}

void print_list(list_t * my_list,void*(*show_list)(void *))
{
    if(my_list == NULL)
    {
        return ;
    }

    list_st * m_list = (list_st *)my_list;

    list_st * temp_list = m_list->next;

    while(temp_list != NULL)
    {
        show_list(temp_list->date);

        temp_list = temp_list->next;
    }
}

void free_list(list_t * my_list)
{
    if(my_list == NULL)
    {
        return ;
    }

    list_st * m_list = (list_st *)my_list;

    list_st * temp = m_list->next;

    while(temp != NULL)
    {
        list_st * temp_list = temp;
    
        free(temp_list);

        temp_list->next = NULL;

        temp_list->front = NULL;
        
        temp = temp->next;
    }

    free(m_list);
    m_list->next = NULL;
    m_list->front = NULL;

}


