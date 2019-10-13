#ifndef _THREAD__H_
#define _THREAD__H_

typedef void pthreadpool_t; 


pthreadpool_t * Inint_pthread_pool(int num);

int add_task(pthreadpool_t * m_pool,void * date,void*(m_task)(void *));

int free_ptherad(pthreadpool_t * m_pool);


#endif
