#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "./scheduler.h"
#include "./queue.h"
#include "./pool.h"
#include "./worker.h"

Scheduler *InitScheduler(enum ThreadPolicy policy, int buffer_size)
{
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));
    if (scheduler == NULL)
    {
        return NULL;
    }
    scheduler->policy = policy;
    scheduler->buffer_size = buffer_size;
    scheduler->curr_size = 0;

    if (policy == FIFO)
    {
        scheduler->queue = InitQueue(buffer_size);
        if (scheduler->queue == NULL)
        {
            return NULL;
        }
    }

    return scheduler;
}

void ScheduleNewRequest(Scheduler *scheduler, int *client_socket)
{

    if (scheduler->policy == FIFO)
    {

        int queue_size = EnQueue(scheduler->queue, client_socket);
        printf("\nQueueSize: %d\n", queue_size);
    }
    scheduler->curr_size++;
}

int *PickRequest(Scheduler *scheduler)
{
    int *client_socket = NULL;
    if (scheduler->policy == FIFO)
    {
        client_socket = DeQueue(scheduler->queue);
    }
    scheduler->curr_size--;
    return client_socket;
}

bool IsSchedulerFull(Scheduler *scheduler)
{
    return scheduler->curr_size == scheduler->buffer_size;
}

bool IsSchedulerEmpty(Scheduler *scheduler)
{
    return scheduler->curr_size == 0;
}

void AddToScheduler(ThreadPool *workers, Scheduler *scheduler, int *client_socket)
{

    int mutex_lock = pthread_mutex_lock(&workers->LOCK);
    if (mutex_lock != 0)
    {
        printf("\n AddToScheduler mutex_lock \n");
    }
    while (IsSchedulerFull(scheduler))
    {
        int cond_wait = pthread_cond_wait(&workers->FILL, &workers->LOCK);
        if (cond_wait != 0)
        {
            printf("\n AddToScheduler cond_wait \n");
        }
    }
    ScheduleNewRequest(scheduler, client_socket);
    int empty_signal = pthread_cond_signal(&workers->EMPTY);
    if (empty_signal != 0)
    {
        printf("\n AddToScheduler empty_signal \n");
    }
    int lock_signal = pthread_mutex_unlock(&workers->LOCK);
    if (lock_signal != 0)
    {
        printf("\n AddToScheduler lock_signal \n");
    }
}

int *GetFromScheduler(ThreadPool *workers, Scheduler *scheduler)
{

    int mutex_lock = pthread_mutex_lock(&workers->LOCK);
    if (mutex_lock != 0)
    {
        printf("\n GetFromScheduler mutex_lock \n");
    }
    while (IsSchedulerEmpty(scheduler))
    {
        int thread_wait = pthread_cond_wait(&workers->EMPTY, &workers->LOCK);
        if (thread_wait != 0)
        {
            printf("\n GetFromScheduler thread_wait \n");
        }
    }
    int *client_socket = PickRequest(scheduler);

    printf("Request Scheduled for FD: %d\n", *client_socket);

    int signal_fill = pthread_cond_signal(&workers->FILL);
    if (signal_fill != 0)
    {
        printf("\n GetFromScheduler signal_fill \n");
    }
    int signal_lock = pthread_mutex_unlock(&workers->LOCK);
    if (signal_lock != 0)
    {
        printf("\n GetFromScheduler signal_lock \n");
    }
    return client_socket;
}