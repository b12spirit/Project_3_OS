
#include "sched_impl.h"
#include "scheduler.h"

/* Fill in your scheduler implementation code below: */

static void init_thread_info(thread_info_t *info, sched_queue_t *queue)
{
        /*...Code goes here...*/
        info->queue = queue->list;
        info->elt = NULL;
        sem_init(&info->cpu_sem, 0, 0);
}
static void destroy_thread_info(thread_info_t *info)
{
        /*...Code goes here...*/
        free(info->elt);
}
/*...More functions go here...*/

static void enter_sched_queue(thread_info_t *info)
{
        sem_wait(&admission_sem);
        info->elt = (list_elem_t *)malloc(sizeof(list_elem_t));
        list_elem_init(info->elt, (void *)info);
        list_insert_tail(info->queue, info->elt);
        if (list_size(info->queue) == 1) //list was previously empty notify wait_for_queue
        {
                sem_post(&ready_sem);
        }
        sem_init(&info->cpu_sem, 0, 0);
}

/* Remove the thread from the scheduler queue. */
static void leave_sched_queue(thread_info_t *info)
{
        list_remove_elem(info->queue, info->elt);
        sem_post(&admission_sem);
}

/* While on the scheduler queue, block until thread is scheduled. */
static void wait_for_cpu(thread_info_t *info)
{
        sem_wait(&info->cpu_sem);
}

/* Voluntarily relinquish the CPU when this thread's timeslice is
 * over (cooperative multithreading). */
static void release_cpu(thread_info_t *info)
{
        sem_post(&cpu_sem);
        sched_yield();
}

//////////////////////////////////////
static void init_sched_queue(sched_queue_t *queue, int queue_size)
{
        /*...Code goes here...*/
        if (queue_size < -1)
        {
                exit(-1); //exit entire program if queue has a size of zero
        }
        queue->current = NULL;
        queue->next = NULL;
        queue->list = (list_t *)malloc(sizeof(list_t));
        list_init(queue->list);
        sem_init(&admission_sem, 0, queue_size);
        sem_init(&cpu_sem, 0, 0);   
        sem_init(&ready_sem, 0, 0);
}

static void destroy_sched_queue(sched_queue_t *queue)
{
        //free(queue->list)
}

/*...More functions go here...*/
static void wake_up_worker(thread_info_t *info)
{
        sem_post(&info->cpu_sem);
}

/* Block until the current worker thread relinquishes the CPU. */
static void wait_for_worker(sched_queue_t *queue)
{
        sem_wait(&cpu_sem);
}

/* Select the next worker thread to execute in round-robin scheduling
 * Returns NULL if the scheduler queue is empty. */
static thread_info_t *next_worker_rr(sched_queue_t *queue)
{
        if (list_size(queue->list) == 0)
        {
                return NULL;
        }

        if (queue->current == NULL)
        { //queue was just empty and now has an item in it
                queue->current = list_get_head(queue->list);
        }
        else if (queue->next == NULL)
        { //the last currentWorker was the tail of the queue
                if (queue->current == list_get_tail(queue->list))
                { //the previous working thread is still in the queue and is the tail
                        queue->current = list_get_head(queue->list);
                }
                else
                {
                        queue->current = list_get_tail(queue->list); //collect the new tail
                }
        }
        else
        { //next worker is a member of the list
                queue->current = queue->next;
        }

        queue->next = queue->current->next;
        return (thread_info_t *)queue->current->datum;
}
static thread_info_t *next_worker_fifo(sched_queue_t *queue)
{
        if (list_size(queue->list) != 0)
        {
                return (thread_info_t *)(list_get_head(queue->list))->datum;
        }
        else
        {
                return NULL;
        }
}

/* Block until at least one worker thread is in the scheduler queue. */
static void wait_for_queue(sched_queue_t *queue)
{
        sem_wait(&ready_sem);
}

/* You need to statically initialize these structures: */
sched_impl_t sched_fifo = {{init_thread_info, destroy_thread_info, enter_sched_queue, leave_sched_queue, wait_for_cpu, release_cpu}, {init_sched_queue, destroy_sched_queue, wake_up_worker, wait_for_worker, next_worker_fifo, wait_for_queue}},
             sched_rr = {{init_thread_info, destroy_thread_info, enter_sched_queue, leave_sched_queue, wait_for_cpu, release_cpu}, {init_sched_queue, destroy_sched_queue, wake_up_worker, wait_for_worker, next_worker_rr, wait_for_queue}};
