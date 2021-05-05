#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__
#include<semaphore.h>
#include "list.h"
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

#endif /* __SCHED_IMPL__H__ */




