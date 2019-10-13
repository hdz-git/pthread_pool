#include "queue_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct queue_str
{
    struct queue_str * front;

    void * date;

    struct queue_str * next;
}queue_st;

queue_t * Inint_queue()
{
    queue_st * queue_list = (queue_st *)malloc(sizeof(queue_st));

    if(queue_list == NULL)
    {
        perror("malloc()");

        return NULL;
    }

    queue_list->next = queue_list;

    queue_list->date = NULL;

    queue_list->front = queue_list;

    return (queue_t *)queue_list;
}

int add_queue(queue_t * queue,void * date)
{
    if(queue == NULL || date == NULL)
    {
        return -1;
    }

    queue_st * queue_list = (queue_st *)queue;

    queue_st * new_queue = (queue_st *)malloc(sizeof(queue_st));

    new_queue->date = date;

    if(queue_list->next != NULL)
    {
        queue_list->next->front = new_queue; 
    }

    new_queue->next = queue_list->next;

    queue_list->next = new_queue;

    new_queue->front = queue_list;

    return 0;
}

void print_queue(queue_t * queue,void * (*show_queue)(void *))
{
    if(queue == NULL || show_queue == NULL)
    {
        return ;
    }

    queue_st * queue_list = (queue_st *)queue;

    queue_st * temp_queue = queue_list->next;

    while(temp_queue != queue_list)
    {
        show_queue(temp_queue->date);

        temp_queue = temp_queue->next;
    }

}




