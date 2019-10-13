#ifndef _QUEUE_LIST__H_
#define _QUEUE_LIST__H_

typedef void queue_t;

queue_t * Inint_queue();
int add_queue(queue_t * queue,void * date);
void print_queue(queue_t * queue,void * (*show_queue)(void *));



#endif
