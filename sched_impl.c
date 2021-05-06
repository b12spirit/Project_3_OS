#include "sched_impl.h"
#include "scheduler.h"
// TEAM : Alonso Vidal, Salvador Leon
/* Fill in your scheduler implementation code below: */
static void init_thread_info(thread_info_t *info, sched_queue_t *queue)
{
        /*...Code goes here...*/
        info->qu = queue->queuelist;
        info->elemt = NULL;
        sem_init(&info->cpu_sem, 0, 0);
}
static void init_sched_queue(sched_queue_t *queue, int queue_size)
{
        if (queue_size < -1)
        {
                exit(-1);
        }
        queue->curr = NULL, queue->next = NULL;
        queue->queuelist = (list_t *)malloc(sizeof(list_t));
        list_init(queue->queuelist);
        sem_init(&admission_sem, 0, queue_size);
        sem_init(&cpu_sem, 0, 0);
        sem_init(&ready_sem, 0, 0);
}
/*...More functions go here...*/
static void enter_sched_queue(thread_info_t *info)
{
        sem_wait(&admission_sem);
        info->elemt = (list_elem_t *)malloc(sizeof(list_elem_t));
        list_insert_tail(info->qu, info->elemt);
        list_elem_init(info->elemt, (void *)info); 
        if (list_size(info->qu) == 1)
        {
                sem_post(&ready_sem);
        }
        sem_init(&info->cpu_sem, 0, 0);
}
static void leave_sched_queue(thread_info_t *info)
{
        list_remove_elem(info->qu, info->elemt);
        sem_post(&admission_sem);
}
static thread_info_t *next_worker_fifo(sched_queue_t *queue)
{
        if (list_size(queue->queuelist) != 0)
        {
                return (thread_info_t *)(list_get_head(queue->queuelist))->datum;
        }
        else
        {
                return NULL;
        }
}
static thread_info_t *next_worker_rr(sched_queue_t *queue)
{
        auto queuedata;
        if (list_size(queue->queuelist) == 0)
        {
                return NULL;
        }

        if (queue->curr == NULL)
        {
                queue->curr = list_get_head(queue->queuelist);
        }
        else if (queue->next != NULL)
        {
                queue->curr = queue->next;
        }
        else
        {
                if (queue->curr == list_get_tail(queue->queuelist))
                {
                        queue->curr = list_get_head(queue->queuelist);
                }
                else
                {
                        queue->curr = list_get_tail(queue->queuelist);
                }
        }
        queue->next = queue->curr->next;
        queuedata = (thread_info_t *)queue->curr->datum;
        return  queuedata;
}
static void wake_up_worker(thread_info_t *info)
{
        sem_post(&info->cpu_sem);
}
static void wait_for_worker(sched_queue_t *queue)
{
        sem_wait(&cpu_sem);
}
static void queueup(sched_queue_t *queue)
{
        sem_wait(&ready_sem);
}
static void wait_for_cpu(thread_info_t *info)
{
        sem_wait(&info->cpu_sem);
}
static void release_cpu(thread_info_t *info)
{
        sem_post(&cpu_sem);
        sched_yield();
}
static void destroy_thread_info(thread_info_t *info)
{
        free(info->elemt);
}
static void destroyqueue(sched_queue_t *queue)
{
        free(queue->queuelist);
}
/* You need to statically initialize these structures: */
sched_impl_t sched_fifo = {{init_thread_info, destroy_thread_info, enter_sched_queue, leave_sched_queue, wait_for_cpu, release_cpu}, {init_sched_queue, destroyqueue, wake_up_worker, wait_for_worker, next_worker_fifo, queueup}},
             sched_rr = {{init_thread_info, destroy_thread_info, enter_sched_queue, leave_sched_queue, wait_for_cpu, release_cpu}, {init_sched_queue, destroyqueue, wake_up_worker, wait_for_worker, next_worker_rr, queueup}};
