#include "sched_impl.h"
#include "scheduler.h"
// TEAM : Alonso Vidal, Salvador Leon
/* Fill in your scheduler implementation code below: */
static void init_thread_info(thread_info_t *info, sched_queue_t *queue)
{
        /*...Code goes here...*/
        info->qu = queue->queuelist;
        info->elemt = NULL;
}
static void init_sched_queue(sched_queue_t *queue, int queue_size)
{
        if (queue_size < -1)
                exit(-1);
        else
        {
                queue->curr = NULL, queue->next = NULL;
                queue->queuelist = (list_t *)malloc(sizeof(list_t));
                if(queue->queuelist != NULL)
                        queue->queuelist->head = queue->queuelist->tail = NULL;
                sem_init(&admission_sem, 0, queue_size);
        }
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
static thread_info_t *nextprocessfifo(sched_queue_t *queue)
{
        if (list_size(queue->queuelist) != 0)
                return (thread_info_t *)(list_get_head(queue->queuelist))->datum;
        else
                return NULL;
}
static thread_info_t *nextprocessrr(sched_queue_t *processqueue)
{
        if (list_size(processqueue->queuelist) == 0)
                return NULL;
        if (processqueue->curr == NULL)
                processqueue->curr = list_get_head(processqueue->queuelist);
        else if (processqueue->next != NULL)
                processqueue->curr = processqueue->next;
        else{
                if (processqueue->curr == list_get_tail(processqueue->queuelist))
                        processqueue->curr = list_get_head(processqueue->queuelist);
                else
                        processqueue->curr = list_get_tail(processqueue->queuelist);
        }
        processqueue->next = processqueue->curr->next;
        return (thread_info_t *)processqueue->curr->datum;
}
static void processtogo(thread_info_t *info)
{
        sem_post(&info->cpu_sem);
}
static void waitingprocess(sched_queue_t *queue)
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
sched_impl_t sched_fifo = {{init_thread_info, destroy_thread_info, enter_sched_queue, leave_sched_queue, wait_for_cpu, release_cpu}, {init_sched_queue, destroyqueue, processtogo, waitingprocess, nextprocessfifo, queueup}},
             sched_rr = {{init_thread_info, destroy_thread_info, enter_sched_queue, leave_sched_queue, wait_for_cpu, release_cpu}, {init_sched_queue, destroyqueue, processtogo, waitingprocess, nextprocessrr, queueup}};
