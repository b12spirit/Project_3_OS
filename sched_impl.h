#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__

struct thread_info {
	/*...Fill this in...*/
	struct sched_queue *queue;
	struct list_elem_t *elt;
	struct sem_t *cpu_sem;
};

struct sched_queue {
	/*...Fill this in...*/
	struct sem_t admission_sem, ready_sem, cpu_sem;
};

#endif /* __SCHED_IMPL__H__ */
