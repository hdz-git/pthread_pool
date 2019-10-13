#include <pthread.h>
#include "thread.h"
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <stdio.h>

//任务管理
typedef struct{
    
    void * ( *show_task)(void *);

    void * date;
    
}task_st;

typedef struct{
    pthread_mutex_t mut;  //互斥量
    pthread_cond_t full_cond;  //满条件变量
    pthread_cond_t empty_cond; //空条件变量

    pthread_t *tid;
    int tid_num;

    task_st task;

    int shutdown;
    int istask;

}pthread_st;

static void *show_pthread(void * arg);

pthreadpool_t * Inint_pthread_pool(int num)
{
    if(num <= 0)
    {
        return NULL;
    }
    
    pthread_st * pool = NULL;
    int err;
    do{
        //初始化pool
        
        pool = (pthread_st *)malloc(sizeof(*pool));

        if(pool == NULL)
        {
            perror("malloc()");
            break;
        }

        pool->tid = NULL;

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

        pool->tid_num = num;

        pool->shutdown = 0;       //1代表线程死亡，0代表线程正常执行

        pool->istask = 0;         //1代表有任务，0代表没有任务

        pool->tid = (pthread_t *)malloc(sizeof(pthread_t) * num);

        if(pool->tid == NULL)
        {
            perror("malloc()");
            break;
        }

        for(int j = 0;j < num;j++)
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

    while(1)
    {
        pthread_mutex_lock(&pool->mut);

        while(pool->istask == 0 && pool->shutdown == 0)
        {
            pthread_cond_wait(&pool->full_cond,&pool->mut);
        }

        if(pool->shutdown == 1)
        {
            pthread_mutex_unlock(&pool->mut);
            break;
        }


        task = pool->task;

        pool->istask = 0;

        pthread_mutex_unlock(&pool->mut);

        pthread_cond_signal(&pool->empty_cond);

        task.show_task(task.date);
    }
    pthread_exit(NULL);
}

int add_task(pthreadpool_t * m_pool,void * date,void*(*m_task)(void *))
{
    if(m_pool == NULL)
    {
        return -1;
    }
    pthread_st * pool = (pthread_st*)m_pool;

    pthread_mutex_lock(&pool->mut);

    while(pool->istask == 1 && pool->shutdown == 0)
    {
        pthread_cond_wait(&pool->empty_cond,&pool->mut);
    }

    if(pool->shutdown == 1)
    {
        pthread_mutex_unlock(&pool->mut);
        return -2;
    }

    pool->task.show_task = m_task;

    pool->task.date = date;

    pool->istask = 1;

    pthread_mutex_unlock(&pool->mut);

    pthread_cond_signal(&pool->full_cond);

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

    pthread_mutex_destroy(&pool->mut);

    pthread_cond_destroy(&pool->full_cond);
    pthread_cond_destroy(&pool->empty_cond);

    free(pool);
    
    return 0;
}


