#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__
#include<semaphore.h>
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <sched.h>
#include <sched.h>
struct thread_info {
	/*...Fill this in...*/
	list_t* queue;
	list_elem_t *elt;
	sem_t cpu_sem;
};

struct sched_queue {
	/*...Fill this in...*/
	list_elem_t *current, *next;
	list_t *list;
};
sem_t admission_sem; //semaphore to control how many threads are in the queue at a time
sem_t cpu_sem;       //To allow 1 thread at a time to use the CPU (acts as mutex);
sem_t ready_sem;    

#endif /* __SCHED_IMPL__H__ */




