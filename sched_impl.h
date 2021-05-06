#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__
#include<semaphore.h>
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <sched.h>
#include <sched.h>
struct thread_info 
{
	sem_t cpu_sem;

	list_t* qu;
	list_elem_t *elemt;
};
struct sched_queue 
{
	list_elem_t *next, *curr;
	list_t *queuelist;
};
sem_t admission_sem;
sem_t cpu_sem;       
sem_t ready_sem;    

#endif /* __SCHED_IMPL__H__ */




