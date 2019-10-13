#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <stdio.h>

#include "mylist.h"
#include "pthreadpool.h"
//任务管理

typedef struct{
    
    void * ( *show_task)(void *);

    void * date;
    
}task_st;

typedef struct{
    pthread_mutex_t mut;  //互斥量
    pthread_cond_t full_cond;  //满条件变量
    pthread_cond_t empty_cond; //空条件变量

    pthread_t *tid;         //线程id
    int tid_num;            //线程数

    int shutdown;           //线程是否死亡 1代表线程死亡 0代表线程正常执行

    list_t * task_pool;             //任务池 
    int task_num;                   //任务数  

    int max_task_num;       //任务池中的最大任务数量

}pthread_st;


static void *show_pthread(void * arg);

pthreadpool_t * Inint_pthread_pool(int pthread_num,int m_max_task_num)
{
    if(pthread_num <= 0 || m_max_task_num <= 0)
    {
        return NULL;
    }
    
    pthread_st * pool = NULL;
    int err;
    do{
        //初始化pool
        
        pool = (pthread_st *)malloc(sizeof(pthread_st));

        if(pool == NULL)
        {
            perror("malloc()");
            break;
        }

        pool->tid = NULL;

        pool->task_pool = NULL;

        err = pthread_mutex_init(&pool->mut,NULL);
        if(err)
        {
            fprintf(stderr,"pthread_mutex_init:%s",strerror(err));
            break;
        }

        err = pthread_cond_init(&pool->full_cond,NULL);
        if(err)
        {    
            fprintf(stderr,"pthread_cond_init:%s",strerror(err));
            break;
        }

        err = pthread_cond_init(&pool->empty_cond,NULL);
        if(err)
        {    
            fprintf(stderr,"pthread_cond_init:%s",strerror(err));
            break;
        }

        pool->tid_num = pthread_num;  //处理任务的线程数

        pool->shutdown = 0;       //1代表线程死亡，0代表线程正常执行

        pool->max_task_num = m_max_task_num;   //初始化任务队列中的最大任务数


        pool->task_pool = Inint_list();   //初始化一个链表，作为任务队列

        pool->task_num = 0;         //初始化任务队列中的任务数

        pool->tid = (pthread_t *)malloc(sizeof(pthread_t) * pthread_num);

        if(pool->tid == NULL)
        {
            perror("malloc()");
            break;
        }

        for(int j = 0;j < pthread_num;j++)
        {
            err = pthread_create(pool->tid+j,NULL,show_pthread,pool);

            if(err)
            {
                break;
            }
        }
        if(err)
        {
            break;
        }

        return (pthreadpool_t *)pool;
        
    }while(0);

    if(pool != NULL)
    {
        if(pool->task_pool != NULL)
        {
            free_list(pool->task_pool);
        }
        if(pool->tid != NULL)
        {
            free(pool->tid);
        }
        free(pool);
    }

    return  NULL;

}

static void *show_pthread(void * arg)
{
    pthread_st * pool = (pthread_st *)arg;

    task_st task;
    task_st * temp_task = NULL;

    while(1)
    {

        pthread_mutex_lock(&pool->mut);

        while((pool->task_num == 0) && (pool->shutdown == 0))
        {
            pthread_cond_wait(&pool->full_cond,&pool->mut);
        }

        if(pool->shutdown == 1)
        {
            pthread_mutex_unlock(&pool->mut);
            break;
        }
 
        list_t * temp_list = del_list(pool->task_pool);

        if(temp_list == NULL)
        {

            pthread_mutex_unlock(&pool->mut);
            break;
        }

        temp_task = (task_st *)temp_list;

        task.show_task = temp_task->show_task;

        task.date = temp_task->date;

        pool->task_num--;
        
        pthread_cond_signal(&pool->empty_cond);

        pthread_mutex_unlock(&pool->mut);

        task.show_task(task.date);
    }
    pthread_exit(NULL);
}

int add_task(pthreadpool_t * m_pool,void * date,void*(*m_task)(void *))
{
    if(m_pool == NULL || date == NULL)
    {
        return -1;
    }

    task_st temp_task;

    pthread_st * pool = (pthread_st*)m_pool;

    pthread_mutex_lock(&pool->mut);

    while((pool->task_num == pool->max_task_num) && (pool->shutdown == 0))   //如果任务队列中的任务达到最大，则睡眠等待
    {

        pthread_cond_wait(&pool->empty_cond,&pool->mut);

    }

    if(pool->shutdown == 1)
    {
        pthread_mutex_unlock(&pool->mut);
        return -2;
    }
    temp_task.show_task = m_task;

    temp_task.date = date;

    add_list(pool->task_pool,(void *)&temp_task);

    pool->task_num++;

    pthread_cond_signal(&pool->full_cond);

    pthread_mutex_unlock(&pool->mut);

    return 0;

}

int free_ptherad(pthreadpool_t * m_pool)
{
    if(m_pool == NULL)
    {
        return -1;
    }
    pthread_st * pool = (pthread_st *)m_pool;

    pool->shutdown = 1;

    for(int i=0;i<pool->tid_num;i++)
    {
        pthread_cond_signal(&pool->full_cond);
    }

    for(int i=0;i<pool->tid_num;i++)
    {
        pthread_join(pool->tid[i],NULL);
    }

    free(pool->tid);

    if(pool->task_pool != NULL)
        free_list(pool->task_pool);

    pthread_mutex_destroy(&pool->mut);

    pthread_cond_destroy(&pool->full_cond);

    pthread_cond_destroy(&pool->empty_cond);

    if(pool != NULL)
        free(pool);
    
    return 0;
}


